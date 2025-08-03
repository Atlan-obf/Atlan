#include "LearningModule.h"
#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>
#include <QtCore/QStandardPaths>
#include <QtCore/QDir>
#include <QtCore/QRandomGenerator>
#include <QtCore/QDateTime>
#include <cmath>

LearningModule::LearningModule(QObject *parent)
    : QObject(parent)
    , learningTimer(new QTimer(this))
    , totalLearningEvents(0)
    , averageConfidence(0.0)
    , maxHistorySize(1000)
    , inputSize(50)
    , hiddenSize(25)
    , outputSize(10)
    , learningRate(0.01)
    , momentum(0.9)
    , isLearning(false)
    , adaptiveMode(true)
{
    initializeLearningSystem();
    
    // Setup continuous learning timer
    connect(learningTimer, &QTimer::timeout, this, &LearningModule::continuousLearning);
    learningTimer->start(10000); // Learn every 10 seconds
    
    // Setup performance evaluation timer
    QTimer *evalTimer = new QTimer(this);
    connect(evalTimer, &QTimer::timeout, this, &LearningModule::evaluatePerformance);
    evalTimer->start(60000); // Evaluate every minute
}

LearningModule::~LearningModule()
{
    saveKnowledge(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/learning_data.json");
}

void LearningModule::initializeLearningSystem()
{
    // Initialize neural network
    initializeNetwork(inputSize, hiddenSize, outputSize);
    
    // Load existing knowledge
    loadKnowledge(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/learning_data.json");
    
    // Initialize with some basic patterns
    knowledgeBase["greeting_patterns"] = QJsonObject{
        {"patterns", QJsonArray{"ahoj", "hello", "hi", "čau", "dobrý deň"}},
        {"confidence", 0.9}
    };
    
    knowledgeBase["programming_patterns"] = QJsonObject{
        {"patterns", QJsonArray{"kód", "program", "funkcia", "trieda", "code", "function", "class"}},
        {"confidence", 0.8}
    };
    
    knowledgeBase["question_patterns"] = QJsonObject{
        {"patterns", QJsonArray{"čo", "ako", "prečo", "kde", "kedy", "what", "how", "why"}},
        {"confidence", 0.7}
    };
}

void LearningModule::learn(const QString &input, const QString &output, double reward)
{
    if (isLearning) {
        return; // Prevent recursive learning
    }
    
    isLearning = true;
    
    // Create learning data entry
    LearningData data;
    data.input = input;
    data.output = output;
    data.context = currentCategory;
    data.reward = reward;
    data.timestamp = QDateTime::currentMSecsSinceEpoch();
    data.category = analyzeCategory(input);
    
    // Add to learning history
    learningHistory.append(data);
    if (learningHistory.size() > maxHistorySize) {
        learningHistory.removeFirst();
    }
    
    // Extract features and train neural network
    QVector<double> inputVector = extractFeatures(input);
    QVector<double> targetVector = extractFeatures(output);
    
    if (inputVector.size() == inputSize && targetVector.size() >= outputSize) {
        QVector<double> target(outputSize);
        for (int i = 0; i < outputSize; ++i) {
            target[i] = (i < targetVector.size()) ? targetVector[i] : 0.0;
        }
        
        backpropagate(inputVector, target);
        updateWeights(learningRate);
    }
    
    // Update pattern recognition
    QStringList patterns = recognizePatterns(input);
    for (const QString &pattern : patterns) {
        patternFrequency[pattern]++;
        
        // Update confidence based on reward
        double currentConfidence = patternConfidence.value(pattern, 0.5);
        double newConfidence = currentConfidence + (reward - 0.5) * 0.1;
        patternConfidence[pattern] = qBound(0.0, newConfidence, 1.0);
    }
    
    totalLearningEvents++;
    
    // Calculate new average confidence
    double totalConfidence = 0.0;
    for (auto it = patternConfidence.begin(); it != patternConfidence.end(); ++it) {
        totalConfidence += it.value();
    }
    averageConfidence = totalConfidence / qMax(1, patternConfidence.size());
    
    // Emit progress update
    int progress = qMin(100, totalLearningEvents / 10);
    emit learningProgressUpdated(progress);
    
    // Update knowledge base
    updateKnowledge(data.category, QJsonObject{
        {"input", input},
        {"output", output},
        {"confidence", patternConfidence.value(data.category, 0.5)},
        {"frequency", patternFrequency.value(data.category, 1)}
    });
    
    emit knowledgeUpdated(data.category);
    isLearning = false;
}

void LearningModule::reinforcementLearning(const QString &action, double reward)
{
    // Simple Q-learning approach
    QString state = currentCategory;
    
    // Update Q-value (simplified)
    QString key = state + "_" + action;
    double currentQ = patternConfidence.value(key, 0.0);
    double newQ = currentQ + learningRate * (reward - currentQ);
    patternConfidence[key] = newQ;
    
    // Adapt learning rate based on performance
    if (adaptiveMode) {
        if (reward > 0.7) {
            learningRate = qMax(0.001, learningRate * 0.99); // Decrease learning rate when doing well
        } else if (reward < 0.3) {
            learningRate = qMin(0.1, learningRate * 1.01); // Increase learning rate when struggling
        }
    }
    
    emit confidenceUpdated(newQ);
}

void LearningModule::unsupervisedLearning(const QStringList &data)
{
    // Cluster similar data points
    QMap<QString, QStringList> clusters;
    
    for (const QString &item : data) {
        QString category = analyzeCategory(item);
        clusters[category].append(item);
    }
    
    // Update knowledge base with discovered patterns
    for (auto it = clusters.begin(); it != clusters.end(); ++it) {
        const QString &category = it.key();
        const QStringList &items = it.value();
        
        QJsonArray patterns;
        for (const QString &item : items) {
            patterns.append(item);
        }
        
        knowledgeBase[category] = QJsonObject{
            {"patterns", patterns},
            {"confidence", 0.6},
            {"discovered", true}
        };
    }
    
    emit learningComplete();
}

QStringList LearningModule::recognizePatterns(const QString &input)
{
    QStringList recognizedPatterns;
    QString lowerInput = input.toLower();
    
    // Check against known patterns in knowledge base
    for (auto it = knowledgeBase.begin(); it != knowledgeBase.end(); ++it) {
        const QString &category = it.key();
        QJsonObject categoryData = it.value().toObject();
        
        if (categoryData.contains("patterns")) {
            QJsonArray patterns = categoryData["patterns"].toArray();
            for (const QJsonValue &patternValue : patterns) {
                QString pattern = patternValue.toString().toLower();
                if (lowerInput.contains(pattern)) {
                    recognizedPatterns.append(category);
                    emit patternRecognized(category);
                    break;
                }
            }
        }
    }
    
    // Use neural network for pattern recognition
    QVector<double> inputVector = extractFeatures(input);
    if (inputVector.size() == inputSize) {
        QVector<double> output = processInput(inputVector);
        
        // Convert neural network output to pattern categories
        for (int i = 0; i < output.size(); ++i) {
            if (output[i] > 0.7) {
                QString neuralPattern = QString("neural_pattern_%1").arg(i);
                if (!recognizedPatterns.contains(neuralPattern)) {
                    recognizedPatterns.append(neuralPattern);
                }
            }
        }
    }
    
    return recognizedPatterns;
}

QString LearningModule::predictOutput(const QString &input)
{
    QStringList patterns = recognizePatterns(input);
    
    if (patterns.isEmpty()) {
        return "Nerozpoznaný vzor - potrebujem sa viac naučiť.";
    }
    
    // Find best matching pattern from learning history
    QString bestOutput;
    double bestConfidence = 0.0;
    
    for (const LearningData &data : learningHistory) {
        QString dataCategory = analyzeCategory(data.input);
        if (patterns.contains(dataCategory)) {
            double confidence = calculateConfidence(input, data.output);
            if (confidence > bestConfidence) {
                bestConfidence = confidence;
                bestOutput = data.output;
            }
        }
    }
    
    if (!bestOutput.isEmpty()) {
        return bestOutput;
    }
    
    // Use neural network prediction
    QVector<double> inputVector = extractFeatures(input);
    if (inputVector.size() == inputSize) {
        QVector<double> output = processInput(inputVector);
        
        // Convert neural network output to text (simplified)
        if (output[0] > 0.8) {
            return "Vysoká zhoda s naučenými vzormi.";
        } else if (output[0] > 0.6) {
            return "Čiastočná zhoda s naučenými vzormi.";
        } else if (output[0] > 0.4) {
            return "Slabá zhoda - možno potrebujem viac informácií.";
        } else {
            return "Neznámy vzor - učím sa novú informáciu.";
        }
    }
    
    return "Nemôžem predpovedať výstup pre tento vstup.";
}

double LearningModule::calculateConfidence(const QString &input, const QString &output)
{
    QStringList inputPatterns = recognizePatterns(input);
    double confidence = 0.0;
    
    for (const QString &pattern : inputPatterns) {
        confidence += patternConfidence.value(pattern, 0.1);
    }
    
    // Normalize by number of patterns
    if (!inputPatterns.isEmpty()) {
        confidence /= inputPatterns.size();
    }
    
    // Factor in neural network confidence
    QVector<double> inputVector = extractFeatures(input);
    if (inputVector.size() == inputSize) {
        QVector<double> networkOutput = processInput(inputVector);
        double networkConfidence = 0.0;
        for (double value : networkOutput) {
            networkConfidence += value;
        }
        networkConfidence /= networkOutput.size();
        
        // Combine rule-based and neural network confidence
        confidence = (confidence + networkConfidence) / 2.0;
    }
    
    return qBound(0.0, confidence, 1.0);
}

void LearningModule::saveKnowledge(const QString &filePath)
{
    QDir().mkpath(QFileInfo(filePath).absolutePath());
    
    QJsonObject root;
    
    // Save knowledge base
    QJsonObject kbObj;
    for (auto it = knowledgeBase.begin(); it != knowledgeBase.end(); ++it) {
        kbObj[it.key()] = it.value();
    }
    root["knowledge_base"] = kbObj;
    
    // Save pattern confidence
    QJsonObject confidenceObj;
    for (auto it = patternConfidence.begin(); it != patternConfidence.end(); ++it) {
        confidenceObj[it.key()] = it.value();
    }
    root["pattern_confidence"] = confidenceObj;
    
    // Save pattern frequency
    QJsonObject frequencyObj;
    for (auto it = patternFrequency.begin(); it != patternFrequency.end(); ++it) {
        frequencyObj[it.key()] = it.value();
    }
    root["pattern_frequency"] = frequencyObj;
    
    // Save neural network weights (simplified)
    QJsonArray weightsArray;
    for (const QVector<double> &layer : weights) {
        QJsonArray layerArray;
        for (double weight : layer) {
            layerArray.append(weight);
        }
        weightsArray.append(layerArray);
    }
    root["neural_weights"] = weightsArray;
    
    // Save biases
    QJsonArray biasesArray;
    for (double bias : biases) {
        biasesArray.append(bias);
    }
    root["neural_biases"] = biasesArray;
    
    // Save learning statistics
    root["total_learning_events"] = totalLearningEvents;
    root["average_confidence"] = averageConfidence;
    root["learning_rate"] = learningRate;
    
    QJsonDocument doc(root);
    
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson());
    }
}

void LearningModule::loadKnowledge(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        return;
    }
    
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    QJsonObject root = doc.object();
    
    // Load knowledge base
    if (root.contains("knowledge_base")) {
        QJsonObject kbObj = root["knowledge_base"].toObject();
        for (auto it = kbObj.begin(); it != kbObj.end(); ++it) {
            knowledgeBase[it.key()] = it.value();
        }
    }
    
    // Load pattern confidence
    if (root.contains("pattern_confidence")) {
        QJsonObject confidenceObj = root["pattern_confidence"].toObject();
        for (auto it = confidenceObj.begin(); it != confidenceObj.end(); ++it) {
            patternConfidence[it.key()] = it.value().toDouble();
        }
    }
    
    // Load pattern frequency
    if (root.contains("pattern_frequency")) {
        QJsonObject frequencyObj = root["pattern_frequency"].toObject();
        for (auto it = frequencyObj.begin(); it != frequencyObj.end(); ++it) {
            patternFrequency[it.key()] = it.value().toInt();
        }
    }
    
    // Load neural network weights
    if (root.contains("neural_weights")) {
        QJsonArray weightsArray = root["neural_weights"].toArray();
        weights.clear();
        for (const QJsonValue &layerValue : weightsArray) {
            QJsonArray layerArray = layerValue.toArray();
            QVector<double> layer;
            for (const QJsonValue &weightValue : layerArray) {
                layer.append(weightValue.toDouble());
            }
            weights.append(layer);
        }
    }
    
    // Load biases
    if (root.contains("neural_biases")) {
        QJsonArray biasesArray = root["neural_biases"].toArray();
        biases.clear();
        for (const QJsonValue &biasValue : biasesArray) {
            biases.append(biasValue.toDouble());
        }
    }
    
    // Load learning statistics
    if (root.contains("total_learning_events")) {
        totalLearningEvents = root["total_learning_events"].toInt();
    }
    if (root.contains("average_confidence")) {
        averageConfidence = root["average_confidence"].toDouble();
    }
    if (root.contains("learning_rate")) {
        learningRate = root["learning_rate"].toDouble();
    }
}

void LearningModule::updateKnowledge(const QString &key, const QJsonObject &data)
{
    knowledgeBase[key] = data;
    emit knowledgeUpdated(key);
}

QJsonObject LearningModule::getKnowledge(const QString &key)
{
    return knowledgeBase.value(key, QJsonObject()).toObject();
}

void LearningModule::initializeNetwork(int inputSize, int hiddenSize, int outputSize)
{
    this->inputSize = inputSize;
    this->hiddenSize = hiddenSize;
    this->outputSize = outputSize;
    
    // Initialize weights with random values
    weights.clear();
    weights.resize(2); // Input to hidden, hidden to output
    
    // Input to hidden weights
    weights[0].resize(hiddenSize * inputSize);
    for (int i = 0; i < hiddenSize * inputSize; ++i) {
        weights[0][i] = (QRandomGenerator::global()->generateDouble() - 0.5) * 2.0;
    }
    
    // Hidden to output weights
    weights[1].resize(outputSize * hiddenSize);
    for (int i = 0; i < outputSize * hiddenSize; ++i) {
        weights[1][i] = (QRandomGenerator::global()->generateDouble() - 0.5) * 2.0;
    }
    
    // Initialize biases
    biases.resize(hiddenSize + outputSize);
    for (int i = 0; i < hiddenSize + outputSize; ++i) {
        biases[i] = (QRandomGenerator::global()->generateDouble() - 0.5) * 2.0;
    }
}

QVector<double> LearningModule::processInput(const QVector<double> &input)
{
    if (input.size() != inputSize || weights.size() < 2) {
        return QVector<double>(outputSize, 0.0);
    }
    
    // Forward pass through the network
    QVector<double> hidden(hiddenSize);
    
    // Input to hidden layer
    for (int i = 0; i < hiddenSize; ++i) {
        double sum = biases[i];
        for (int j = 0; j < inputSize; ++j) {
            sum += weights[0][i * inputSize + j] * input[j];
        }
        hidden[i] = activationFunction(sum);
    }
    
    // Hidden to output layer
    QVector<double> output(outputSize);
    for (int i = 0; i < outputSize; ++i) {
        double sum = biases[hiddenSize + i];
        for (int j = 0; j < hiddenSize; ++j) {
            sum += weights[1][i * hiddenSize + j] * hidden[j];
        }
        output[i] = activationFunction(sum);
    }
    
    lastOutput = output;
    lastHidden = hidden;
    
    return output;
}

void LearningModule::trainNetwork(const QVector<QVector<double>> &inputs, 
                                 const QVector<QVector<double>> &targets)
{
    for (int epoch = 0; epoch < inputs.size(); ++epoch) {
        if (inputs[epoch].size() == inputSize && targets[epoch].size() == outputSize) {
            processInput(inputs[epoch]);
            backpropagate(inputs[epoch], targets[epoch]);
            updateWeights(learningRate);
        }
    }
}

void LearningModule::analyzeMistakes()
{
    // Analyze recent learning data for patterns in mistakes
    QMap<QString, int> errorPatterns;
    
    for (const LearningData &data : learningHistory) {
        if (data.reward < 0.5) { // Consider as mistake
            QString category = analyzeCategory(data.input);
            errorPatterns[category]++;
        }
    }
    
    // Adjust confidence for problematic patterns
    for (auto it = errorPatterns.begin(); it != errorPatterns.end(); ++it) {
        const QString &pattern = it.key();
        int errorCount = it.value();
        
        if (errorCount > 3) { // Frequent mistakes
            patternConfidence[pattern] = qMax(0.1, patternConfidence.value(pattern, 0.5) - 0.2);
            emit errorInLearning(QString("Časté chyby v kategórii: %1").arg(pattern));
        }
    }
}

void LearningModule::optimizePerformance()
{
    // Prune low-confidence patterns
    QStringList toRemove;
    for (auto it = patternConfidence.begin(); it != patternConfidence.end(); ++it) {
        if (it.value() < 0.1 && patternFrequency.value(it.key(), 0) < 2) {
            toRemove.append(it.key());
        }
    }
    
    for (const QString &pattern : toRemove) {
        patternConfidence.remove(pattern);
        patternFrequency.remove(pattern);
    }
    
    // Consolidate similar patterns
    // (This would require more sophisticated similarity metrics)
}

void LearningModule::adaptToNewPatterns()
{
    // Increase learning rate for new patterns
    if (adaptiveMode) {
        // Recent learning events
        int recentEvents = 0;
        qint64 recentTime = QDateTime::currentMSecsSinceEpoch() - 300000; // Last 5 minutes
        
        for (const LearningData &data : learningHistory) {
            if (data.timestamp > recentTime) {
                recentEvents++;
            }
        }
        
        if (recentEvents > 10) {
            learningRate = qMin(0.05, learningRate * 1.1);
        }
    }
}

int LearningModule::getTotalLearningEvents() const
{
    return totalLearningEvents;
}

double LearningModule::getAverageConfidence() const
{
    return averageConfidence;
}

QStringList LearningModule::getMostLearnedPatterns() const
{
    QStringList patterns;
    
    // Sort patterns by frequency
    QList<QPair<int, QString>> sortedPatterns;
    for (auto it = patternFrequency.begin(); it != patternFrequency.end(); ++it) {
        sortedPatterns.append(qMakePair(it.value(), it.key()));
    }
    
    std::sort(sortedPatterns.begin(), sortedPatterns.end(), 
              [](const QPair<int, QString> &a, const QPair<int, QString> &b) {
                  return a.first > b.first;
              });
    
    for (int i = 0; i < qMin(10, sortedPatterns.size()); ++i) {
        patterns.append(sortedPatterns[i].second);
    }
    
    return patterns;
}

QString LearningModule::getLearningReport() const
{
    QString report = QString("=== SPRÁVA O UČENÍ ===\n\n");
    report += QString("Celkový počet učebných udalostí: %1\n").arg(totalLearningEvents);
    report += QString("Priemerná spoľahlivosť: %1%\n").arg(averageConfidence * 100, 0, 'f', 1);
    report += QString("Aktuálna rýchlosť učenia: %1\n").arg(learningRate, 0, 'f', 4);
    report += QString("Počet naučených vzorov: %1\n\n").arg(patternConfidence.size());
    
    report += "Najčastejšie vzory:\n";
    QStringList topPatterns = getMostLearnedPatterns();
    for (int i = 0; i < topPatterns.size(); ++i) {
        const QString &pattern = topPatterns[i];
        report += QString("%1. %2 (frekvencia: %3, spoľahlivosť: %4%)\n")
                  .arg(i + 1)
                  .arg(pattern)
                  .arg(patternFrequency.value(pattern, 0))
                  .arg(patternConfidence.value(pattern, 0.0) * 100, 0, 'f', 1);
    }
    
    return report;
}

void LearningModule::continuousLearning()
{
    if (learningHistory.size() < 2) {
        return;
    }
    
    // Self-reinforcement learning from recent interactions
    int recentCount = qMin(5, learningHistory.size());
    for (int i = learningHistory.size() - recentCount; i < learningHistory.size(); ++i) {
        const LearningData &data = learningHistory[i];
        
        // Reinforce successful patterns
        if (data.reward > 0.7) {
            QString category = analyzeCategory(data.input);
            patternConfidence[category] = qMin(1.0, patternConfidence.value(category, 0.5) + 0.05);
        }
    }
    
    // Adapt to new patterns
    adaptToNewPatterns();
    
    // Emit progress update
    int progress = qMin(100, (totalLearningEvents * patternConfidence.size()) / 100);
    emit learningProgressUpdated(progress);
}

void LearningModule::evaluatePerformance()
{
    analyzeMistakes();
    optimizePerformance();
    
    // Update average confidence
    double totalConfidence = 0.0;
    for (auto it = patternConfidence.begin(); it != patternConfidence.end(); ++it) {
        totalConfidence += it.value();
    }
    averageConfidence = totalConfidence / qMax(1, patternConfidence.size());
    
    emit confidenceUpdated(averageConfidence);
}

QStringList LearningModule::extractFeatures(const QString &input)
{
    QStringList features;
    QString lowerInput = input.toLower();
    
    // Extract word features
    QStringList words = lowerInput.split(QRegularExpression("\\W+"), Qt::SkipEmptyParts);
    
    // Limit to inputSize features
    for (int i = 0; i < qMin(inputSize, words.size()); ++i) {
        features.append(words[i]);
    }
    
    // Pad with empty strings if needed
    while (features.size() < inputSize) {
        features.append("");
    }
    
    return features;
}

QString LearningModule::findSimilarPatterns(const QString &input)
{
    QStringList inputFeatures = extractFeatures(input);
    QString mostSimilar;
    double bestSimilarity = 0.0;
    
    for (const LearningData &data : learningHistory) {
        QStringList dataFeatures = extractFeatures(data.input);
        
        // Calculate similarity (simplified Jaccard similarity)
        QSet<QString> inputSet = QSet<QString>(inputFeatures.begin(), inputFeatures.end());
        QSet<QString> dataSet = QSet<QString>(dataFeatures.begin(), dataFeatures.end());
        
        int intersection = (inputSet & dataSet).size();
        int unionSize = (inputSet | dataSet).size();
        
        double similarity = (unionSize > 0) ? static_cast<double>(intersection) / unionSize : 0.0;
        
        if (similarity > bestSimilarity) {
            bestSimilarity = similarity;
            mostSimilar = data.input;
        }
    }
    
    return (bestSimilarity > 0.3) ? mostSimilar : QString();
}

void LearningModule::clusterData()
{
    // Simple clustering based on categories
    QMap<QString, QStringList> clusters;
    
    for (const LearningData &data : learningHistory) {
        QString category = analyzeCategory(data.input);
        clusters[category].append(data.input);
    }
    
    // Update knowledge base with clusters
    for (auto it = clusters.begin(); it != clusters.end(); ++it) {
        const QString &category = it.key();
        const QStringList &items = it.value();
        
        if (items.size() > 2) { // Only create clusters with multiple items
            QJsonArray patterns;
            for (const QString &item : items) {
                patterns.append(item);
            }
            
            knowledgeBase[category + "_cluster"] = QJsonObject{
                {"patterns", patterns},
                {"confidence", 0.6},
                {"cluster_size", items.size()}
            };
        }
    }
}

double LearningModule::activationFunction(double x)
{
    // Sigmoid activation function
    return 1.0 / (1.0 + std::exp(-x));
}

double LearningModule::activationDerivative(double x)
{
    return x * (1.0 - x);
}

void LearningModule::backpropagate(const QVector<double> &input, const QVector<double> &target)
{
    if (input.size() != inputSize || target.size() != outputSize || 
        lastOutput.size() != outputSize || lastHidden.size() != hiddenSize) {
        return;
    }
    
    // Calculate output layer errors
    QVector<double> outputErrors(outputSize);
    for (int i = 0; i < outputSize; ++i) {
        double error = target[i] - lastOutput[i];
        outputErrors[i] = error * activationDerivative(lastOutput[i]);
    }
    
    // Calculate hidden layer errors
    QVector<double> hiddenErrors(hiddenSize);
    for (int i = 0; i < hiddenSize; ++i) {
        double error = 0.0;
        for (int j = 0; j < outputSize; ++j) {
            error += outputErrors[j] * weights[1][j * hiddenSize + i];
        }
        hiddenErrors[i] = error * activationDerivative(lastHidden[i]);
    }
    
    // Store errors for weight updates
    // (In a full implementation, you would store these for batch updates)
}

void LearningModule::updateWeights(double learningRate)
{
    // This is a simplified weight update
    // In a full implementation, you would use the stored errors from backpropagation
    
    // Add small random changes to simulate learning
    for (int layer = 0; layer < weights.size(); ++layer) {
        for (int i = 0; i < weights[layer].size(); ++i) {
            double change = (QRandomGenerator::global()->generateDouble() - 0.5) * learningRate * 0.1;
            weights[layer][i] += change;
        }
    }
    
    // Update biases
    for (int i = 0; i < biases.size(); ++i) {
        double change = (QRandomGenerator::global()->generateDouble() - 0.5) * learningRate * 0.1;
        biases[i] += change;
    }
}

QString LearningModule::analyzeCategory(const QString &input)
{
    QString lowerInput = input.toLower();
    
    // Simple category analysis based on keywords
    if (lowerInput.contains(QRegularExpression("\\b(ahoj|hello|hi|čau|dobrý)\\b"))) {
        return "greeting";
    } else if (lowerInput.contains(QRegularExpression("\\b(kód|program|funkcia|trieda|code|function|class)\\b"))) {
        return "programming";
    } else if (lowerInput.contains(QRegularExpression("\\b(čo|ako|prečo|kde|kedy|what|how|why|where|when)\\b"))) {
        return "question";
    } else if (lowerInput.contains(QRegularExpression("\\b(ďakujem|thanks|thank you|vďaka)\\b"))) {
        return "gratitude";
    } else if (lowerInput.contains(QRegularExpression("\\b(pomoc|help|assist)\\b"))) {
        return "help_request";
    } else {
        return "general";
    }
}

#include "LearningModule.moc"