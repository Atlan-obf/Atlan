#ifndef LEARNINGMODULE_H
#define LEARNINGMODULE_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QMap>
#include <QtCore/QVector>
#include <QtCore/QTimer>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>

struct LearningData {
    QString input;
    QString output;
    QString context;
    double reward;
    qint64 timestamp;
    QString category;
};

struct NeuralConnection {
    int fromNode;
    int toNode;
    double weight;
    double lastUpdate;
};

class LearningModule : public QObject
{
    Q_OBJECT

public:
    explicit LearningModule(QObject *parent = nullptr);
    ~LearningModule();
    
    // Learning methods
    void learn(const QString &input, const QString &output, double reward = 1.0);
    void reinforcementLearning(const QString &action, double reward);
    void unsupervisedLearning(const QStringList &data);
    
    // Pattern recognition
    QStringList recognizePatterns(const QString &input);
    QString predictOutput(const QString &input);
    double calculateConfidence(const QString &input, const QString &output);
    
    // Knowledge management
    void saveKnowledge(const QString &filePath);
    void loadKnowledge(const QString &filePath);
    void updateKnowledge(const QString &key, const QJsonObject &data);
    QJsonObject getKnowledge(const QString &key);
    
    // Neural network operations
    void initializeNetwork(int inputSize, int hiddenSize, int outputSize);
    QVector<double> processInput(const QVector<double> &input);
    void trainNetwork(const QVector<QVector<double>> &inputs, 
                     const QVector<QVector<double>> &targets);
    
    // Self-improvement
    void analyzeMistakes();
    void optimizePerformance();
    void adaptToNewPatterns();
    
    // Statistics and monitoring
    int getTotalLearningEvents() const;
    double getAverageConfidence() const;
    QStringList getMostLearnedPatterns() const;
    QString getLearningReport() const;

signals:
    void learningProgressUpdated(int progress);
    void patternRecognized(const QString &pattern);
    void confidenceUpdated(double confidence);
    void knowledgeUpdated(const QString &category);
    void learningComplete();
    void errorInLearning(const QString &error);

private slots:
    void continuousLearning();
    void evaluatePerformance();

private:
    void initializeLearningSystem();
    void processLearningData();
    void updateNeuralConnections();
    
    // Pattern analysis
    QStringList extractFeatures(const QString &input);
    QString findSimilarPatterns(const QString &input);
    void clusterData();
    
    // Neural network helpers
    double activationFunction(double x);
    double activationDerivative(double x);
    void backpropagate(const QVector<double> &input, const QVector<double> &target);
    void updateWeights(double learningRate);
    
    // Data structures
    QVector<LearningData> learningHistory;
    QVector<NeuralConnection> connections;
    QMap<QString, QJsonObject> knowledgeBase;
    QMap<QString, double> patternConfidence;
    QMap<QString, int> patternFrequency;
    
    // Neural network
    QVector<QVector<double>> weights;
    QVector<double> biases;
    QVector<double> lastOutput;
    QVector<double> lastHidden;
    
    int inputSize;
    int hiddenSize;
    int outputSize;
    double learningRate;
    double momentum;
    
    // Learning parameters
    QTimer *learningTimer;
    int totalLearningEvents;
    double averageConfidence;
    int maxHistorySize;
    
    bool isLearning;
    bool adaptiveMode;
    QString currentCategory;
};

#endif // LEARNINGMODULE_H