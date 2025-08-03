#include "AIEngine.h"
#include "NetworkManager.h"
#include "LearningModule.h"

#include <QtCore/QDebug>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QStandardPaths>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QRegularExpression>
#include <QtCore/QRandomGenerator>
#include <cmath>

AIEngine::AIEngine(QObject *parent)
    : QObject(parent)
    , networkManager(nullptr)
    , learningModule(nullptr)
    , learningTimer(new QTimer(this))
    , isProcessing(false)
    , inputSize(100)
    , hiddenSize(50)
    , outputSize(20)
{
    // Initialize context
    context.contextLength = 10;
    
    // Setup learning timer
    connect(learningTimer, &QTimer::timeout, this, &AIEngine::onLearningUpdate);
    learningTimer->start(5000); // Update every 5 seconds
    
    initializeKnowledgeBase();
    initializeNeuralNetwork();
}

AIEngine::~AIEngine()
{
    saveKnowledgeBase();
}

void AIEngine::initialize()
{
    emit statusChanged("Inicializujem AI systém...");
    
    loadKnowledgeBase();
    
    // Initialize with some basic knowledge
    knowledgeBase.facts["greeting"] = "Ahoj! Som AI asistent. Môžem vám pomôcť s programovaním, odpovedať na otázky a učiť sa z našej konverzácie.";
    knowledgeBase.facts["programming"] = "Môžem generovať kód v C++, Python, JavaScript a ďalších jazykoch.";
    knowledgeBase.facts["learning"] = "Učím sa z každej interakcie a postupne sa zlepšujem.";
    
    // Add some code examples
    knowledgeBase.codeExamples << 
        "#include <iostream>\nusing namespace std;\nint main() {\n    cout << \"Hello World!\" << endl;\n    return 0;\n}";
    
    knowledgeBase.codeExamples <<
        "def hello_world():\n    print(\"Hello World!\")\n\nhello_world()";
    
    emit statusChanged("AI systém inicializovaný");
}

void AIEngine::processMessage(const QString &message)
{
    if (isProcessing) {
        emit errorOccurred("AI už spracúva inú správu");
        return;
    }
    
    isProcessing = true;
    currentQuery = message;
    emit statusChanged("Analyzujem správu...");
    
    // Add to context
    context.messages.append(message);
    if (context.messages.size() > context.contextLength) {
        context.messages.removeFirst();
    }
    
    // Analyze input
    QString analysis = analyzeInput(message);
    
    // Generate response
    QString response = generateResponse(message);
    
    // Learn from interaction
    if (learningModule) {
        learningModule->learn(message, response);
    }
    
    // Add to context
    context.responses.append(response);
    if (context.responses.size() > context.contextLength) {
        context.responses.removeFirst();
    }
    
    isProcessing = false;
    emit responseReady(response);
    emit statusChanged("Pripravený");
}

void AIEngine::setNetworkManager(NetworkManager *manager)
{
    networkManager = manager;
    if (networkManager) {
        connect(networkManager, &NetworkManager::aiResponseReady,
                this, &AIEngine::processNetworkResponse);
    }
}

void AIEngine::setLearningModule(LearningModule *module)
{
    learningModule = module;
}

void AIEngine::learnFromInteraction(const QString &input, const QString &output)
{
    // Extract patterns from the interaction
    QStringList inputTokens = tokenize(input);
    QStringList outputTokens = tokenize(output);
    
    // Update knowledge base
    for (const QString &token : inputTokens) {
        if (!knowledgeBase.patterns.contains(token)) {
            knowledgeBase.patterns[token] = QStringList();
        }
        knowledgeBase.patterns[token].append(output);
        
        // Update confidence
        knowledgeBase.confidence[token] = qMin(1.0, knowledgeBase.confidence.value(token, 0.5) + 0.1);
    }
    
    // Simulate neural network learning
    QVector<double> input_vector(inputSize);
    QVector<double> target_vector(outputSize);
    
    // Convert text to numerical representation (simplified)
    for (int i = 0; i < qMin(inputTokens.size(), inputSize); ++i) {
        input_vector[i] = inputTokens[i].length() / 10.0; // Simple encoding
    }
    
    for (int i = 0; i < qMin(outputTokens.size(), outputSize); ++i) {
        target_vector[i] = outputTokens[i].length() / 10.0;
    }
    
    backpropagate(input_vector, target_vector);
    
    emit learningProgressUpdated(qMin(100, static_cast<int>(knowledgeBase.confidence.size())));
}

void AIEngine::updateKnowledgeBase(const QString &topic, const QString &information)
{
    knowledgeBase.facts[topic] = information;
    emit statusChanged("Vedomostná báza aktualizovaná");
}

QString AIEngine::generateResponse(const QString &input)
{
    QString processedInput = preprocessText(input);
    QStringList tokens = tokenize(processedInput);
    
    // Check for greetings
    if (processedInput.contains(QRegularExpression("(ahoj|hello|hi|čau|dobrý)"))) {
        return knowledgeBase.facts.value("greeting", "Ahoj! Ako vám môžem pomôcť?");
    }
    
    // Check for programming questions
    if (processedInput.contains(QRegularExpression("(kód|program|funkci|trieda|class|function)"))) {
        return "Môžem vám pomôcť s programovaním! Aký typ kódu potrebujete? Špecifikujte jazyk a čo má kód robiť.";
    }
    
    // Check for code generation requests
    if (processedInput.contains(QRegularExpression("(vytvor|generuj|napíš|create|generate)"))) {
        return generateCode(input);
    }
    
    // Try to find best response from patterns
    QString bestResponse = findBestResponse(input);
    if (!bestResponse.isEmpty()) {
        return bestResponse;
    }
    
    // Use neural network for response generation
    QVector<double> inputVector(inputSize, 0.0);
    for (int i = 0; i < qMin(tokens.size(), inputSize); ++i) {
        inputVector[i] = tokens[i].length() / 10.0; // Simple text encoding
    }
    
    QVector<double> output = forwardPass(inputVector);
    
    // Convert neural network output to text (simplified)
    if (output[0] > 0.7) {
        return "To je zaujímavá otázka. Môžete mi poskytnúť viac detailov?";
    } else if (output[0] > 0.5) {
        return "Rozumiem vašej otázke. Skúsim vám pomôcť na základe toho, čo som sa naučil.";
    } else if (output[0] > 0.3) {
        return "Hmm, nie som si istý. Môžete preformulovať otázku?";
    } else {
        return "Prepáčte, ale nerozumiem úplne. Môžete mi to vysvetliť inak?";
    }
}

QString AIEngine::generateCode(const QString &description, const QString &language)
{
    QString lowerDesc = description.toLower();
    QString code;
    
    if (language == "cpp" || lowerDesc.contains("c++")) {
        // Generate C++ code based on description
        if (lowerDesc.contains("hello world") || lowerDesc.contains("hello")) {
            code = "#include <iostream>\n"
                   "using namespace std;\n\n"
                   "int main() {\n"
                   "    cout << \"Hello World!\" << endl;\n"
                   "    return 0;\n"
                   "}";
        }
        else if (lowerDesc.contains("kalkulačka") || lowerDesc.contains("calculator")) {
            code = "#include <iostream>\n"
                   "using namespace std;\n\n"
                   "int main() {\n"
                   "    double a, b;\n"
                   "    char op;\n"
                   "    \n"
                   "    cout << \"Zadajte prvé číslo: \";\n"
                   "    cin >> a;\n"
                   "    cout << \"Zadajte operátor (+, -, *, /): \";\n"
                   "    cin >> op;\n"
                   "    cout << \"Zadajte druhé číslo: \";\n"
                   "    cin >> b;\n"
                   "    \n"
                   "    switch(op) {\n"
                   "        case '+': cout << \"Výsledok: \" << a + b << endl; break;\n"
                   "        case '-': cout << \"Výsledok: \" << a - b << endl; break;\n"
                   "        case '*': cout << \"Výsledok: \" << a * b << endl; break;\n"
                   "        case '/': \n"
                   "            if(b != 0) cout << \"Výsledok: \" << a / b << endl;\n"
                   "            else cout << \"Chyba: Delenie nulou!\" << endl;\n"
                   "            break;\n"
                   "        default: cout << \"Neznámy operátor!\" << endl;\n"
                   "    }\n"
                   "    \n"
                   "    return 0;\n"
                   "}";
        }
        else if (lowerDesc.contains("trieda") || lowerDesc.contains("class")) {
            code = "#include <iostream>\n"
                   "#include <string>\n"
                   "using namespace std;\n\n"
                   "class MyClass {\n"
                   "private:\n"
                   "    string name;\n"
                   "    int value;\n"
                   "\n"
                   "public:\n"
                   "    MyClass(string n, int v) : name(n), value(v) {}\n"
                   "    \n"
                   "    void display() {\n"
                   "        cout << \"Meno: \" << name << \", Hodnota: \" << value << endl;\n"
                   "    }\n"
                   "    \n"
                   "    void setValue(int v) { value = v; }\n"
                   "    int getValue() const { return value; }\n"
                   "};\n\n"
                   "int main() {\n"
                   "    MyClass obj(\"Test\", 42);\n"
                   "    obj.display();\n"
                   "    return 0;\n"
                   "}";
        }
        else {
            code = "// Generovaný C++ kód pre: " + description + "\n"
                   "#include <iostream>\n"
                   "using namespace std;\n\n"
                   "int main() {\n"
                   "    // TODO: Implementujte požadovanú funkcionalitu\n"
                   "    cout << \"Program spustený!\" << endl;\n"
                   "    return 0;\n"
                   "}";
        }
    }
    else if (language == "python" || lowerDesc.contains("python")) {
        if (lowerDesc.contains("hello world") || lowerDesc.contains("hello")) {
            code = "def main():\n"
                   "    print(\"Hello World!\")\n\n"
                   "if __name__ == \"__main__\":\n"
                   "    main()";
        }
        else {
            code = "# Generovaný Python kód pre: " + description + "\n"
                   "def main():\n"
                   "    # TODO: Implementujte požadovanú funkcionalitu\n"
                   "    print(\"Program spustený!\")\n\n"
                   "if __name__ == \"__main__\":\n"
                   "    main()";
        }
    }
    else {
        // Default to C++
        code = generateCode(description, "cpp");
    }
    
    // Learn from generated code
    learnFromInteraction(description, code);
    
    emit codeGenerated(code);
    return code;
}

bool AIEngine::validateCode(const QString &code, const QString &language)
{
    // Basic syntax validation (simplified)
    if (language == "cpp") {
        return code.contains("#include") && code.contains("main");
    } else if (language == "python") {
        return !code.contains(QRegularExpression("^\\s*def\\s+\\w+\\s*\\(.*\\)\\s*:"));
    }
    return true;
}

void AIEngine::addToContext(const QString &message, const QString &response)
{
    context.messages.append(message);
    context.responses.append(response);
    
    if (context.messages.size() > context.contextLength) {
        context.messages.removeFirst();
        context.responses.removeFirst();
    }
}

void AIEngine::clearContext()
{
    context.messages.clear();
    context.responses.clear();
    context.currentTopic.clear();
}

QString AIEngine::getContextSummary()
{
    QString summary = "Posledné správy:\n";
    for (int i = 0; i < context.messages.size(); ++i) {
        summary += QString("Používateľ: %1\nAI: %2\n\n")
                   .arg(context.messages[i])
                   .arg(context.responses.value(i, ""));
    }
    return summary;
}

void AIEngine::processNetworkResponse(const QString &response)
{
    // Process response from network (e.g., from external AI API)
    emit responseReady("Odpoveď zo siete: " + response);
}

void AIEngine::onLearningUpdate()
{
    // Periodic learning updates
    if (knowledgeBase.facts.size() > 0) {
        emit learningProgressUpdated(qMin(100, knowledgeBase.facts.size() * 5));
    }
}

void AIEngine::initializeKnowledgeBase()
{
    // Initialize with empty knowledge base
    knowledgeBase.facts.clear();
    knowledgeBase.patterns.clear();
    knowledgeBase.confidence.clear();
    knowledgeBase.codeExamples.clear();
}

void AIEngine::saveKnowledgeBase()
{
    QString dataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(dataPath);
    
    QFile file(dataPath + "/knowledge.json");
    if (file.open(QIODevice::WriteOnly)) {
        QJsonObject json;
        
        // Save facts
        QJsonObject factsObj;
        for (auto it = knowledgeBase.facts.begin(); it != knowledgeBase.facts.end(); ++it) {
            factsObj[it.key()] = it.value();
        }
        json["facts"] = factsObj;
        
        // Save confidence scores
        QJsonObject confidenceObj;
        for (auto it = knowledgeBase.confidence.begin(); it != knowledgeBase.confidence.end(); ++it) {
            confidenceObj[it.key()] = it.value();
        }
        json["confidence"] = confidenceObj;
        
        QJsonDocument doc(json);
        file.write(doc.toJson());
    }
}

void AIEngine::loadKnowledgeBase()
{
    QString dataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QFile file(dataPath + "/knowledge.json");
    
    if (file.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        QJsonObject json = doc.object();
        
        // Load facts
        QJsonObject factsObj = json["facts"].toObject();
        for (auto it = factsObj.begin(); it != factsObj.end(); ++it) {
            knowledgeBase.facts[it.key()] = it.value().toString();
        }
        
        // Load confidence scores
        QJsonObject confidenceObj = json["confidence"].toObject();
        for (auto it = confidenceObj.begin(); it != confidenceObj.end(); ++it) {
            knowledgeBase.confidence[it.key()] = it.value().toDouble();
        }
    }
}

QString AIEngine::analyzeInput(const QString &input)
{
    QStringList tokens = tokenize(input);
    QString analysis = "Analýza: ";
    
    // Detect question words
    QStringList questionWords = {"čo", "ako", "prečo", "kde", "kedy", "kto", "what", "how", "why", "where", "when", "who"};
    for (const QString &word : questionWords) {
        if (tokens.contains(word, Qt::CaseInsensitive)) {
            analysis += "otázka, ";
            break;
        }
    }
    
    // Detect programming keywords
    QStringList progWords = {"kód", "program", "funkcia", "trieda", "code", "function", "class"};
    for (const QString &word : progWords) {
        if (tokens.contains(word, Qt::CaseInsensitive)) {
            analysis += "programovanie, ";
            break;
        }
    }
    
    return analysis;
}

QString AIEngine::findBestResponse(const QString &input)
{
    QStringList tokens = tokenize(input);
    QString bestResponse;
    double bestScore = 0.0;
    
    for (const QString &token : tokens) {
        if (knowledgeBase.patterns.contains(token)) {
            const QStringList &responses = knowledgeBase.patterns[token];
            for (const QString &response : responses) {
                double score = knowledgeBase.confidence.value(token, 0.0);
                if (score > bestScore) {
                    bestScore = score;
                    bestResponse = response;
                }
            }
        }
    }
    
    return bestResponse;
}

double AIEngine::calculateConfidence(const QString &input, const QString &response)
{
    QStringList inputTokens = tokenize(input);
    double confidence = 0.0;
    
    for (const QString &token : inputTokens) {
        confidence += knowledgeBase.confidence.value(token, 0.1);
    }
    
    return confidence / qMax(1, inputTokens.size());
}

QStringList AIEngine::tokenize(const QString &text)
{
    return text.toLower()
               .split(QRegularExpression("\\W+"), Qt::SkipEmptyParts);
}

QString AIEngine::preprocessText(const QString &text)
{
    QString processed = text.toLower();
    
    // Remove special characters but keep basic punctuation
    processed.remove(QRegularExpression("[^\\w\\s\\?\\!\\.]"));
    
    // Normalize whitespace
    processed = processed.simplified();
    
    return processed;
}

void AIEngine::initializeNeuralNetwork()
{
    // Initialize weights with random values
    weights.resize(hiddenSize);
    for (int i = 0; i < hiddenSize; ++i) {
        weights[i].resize(inputSize);
        for (int j = 0; j < inputSize; ++j) {
            weights[i][j] = (QRandomGenerator::global()->generateDouble() - 0.5) * 2.0;
        }
    }
    
    // Initialize biases
    biases.resize(hiddenSize);
    for (int i = 0; i < hiddenSize; ++i) {
        biases[i] = (QRandomGenerator::global()->generateDouble() - 0.5) * 2.0;
    }
}

QVector<double> AIEngine::forwardPass(const QVector<double> &input)
{
    QVector<double> hidden(hiddenSize);
    
    // Calculate hidden layer
    for (int i = 0; i < hiddenSize; ++i) {
        double sum = biases[i];
        for (int j = 0; j < qMin(input.size(), inputSize); ++j) {
            sum += weights[i][j] * input[j];
        }
        hidden[i] = sigmoid(sum);
    }
    
    // Simple output layer (just take first few hidden neurons as output)
    QVector<double> output(outputSize);
    for (int i = 0; i < outputSize && i < hiddenSize; ++i) {
        output[i] = hidden[i];
    }
    
    return output;
}

void AIEngine::backpropagate(const QVector<double> &input, const QVector<double> &target)
{
    // Simplified backpropagation
    QVector<double> output = forwardPass(input);
    double learningRate = 0.01;
    
    // Calculate error and update weights (simplified)
    for (int i = 0; i < hiddenSize && i < target.size(); ++i) {
        double error = target[i] - output[qMin(i, output.size() - 1)];
        
        for (int j = 0; j < qMin(input.size(), inputSize); ++j) {
            weights[i][j] += learningRate * error * input[j] * sigmoidDerivative(output[qMin(i, output.size() - 1)]);
        }
        
        biases[i] += learningRate * error * sigmoidDerivative(output[qMin(i, output.size() - 1)]);
    }
}

double AIEngine::sigmoid(double x)
{
    return 1.0 / (1.0 + std::exp(-x));
}

double AIEngine::sigmoidDerivative(double x)
{
    return x * (1.0 - x);
}

#include "AIEngine.moc"