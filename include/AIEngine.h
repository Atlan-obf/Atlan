#ifndef AIENGINE_H
#define AIENGINE_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QMap>
#include <QtCore/QVector>
#include <QtCore/QTimer>
#include <QtCore/QThread>
#include <memory>

class NetworkManager;
class LearningModule;

struct ConversationContext {
    QStringList messages;
    QStringList responses;
    QString currentTopic;
    int contextLength;
};

struct KnowledgeBase {
    QMap<QString, QString> facts;
    QMap<QString, QStringList> patterns;
    QMap<QString, double> confidence;
    QStringList codeExamples;
};

class AIEngine : public QObject
{
    Q_OBJECT

public:
    explicit AIEngine(QObject *parent = nullptr);
    ~AIEngine();
    
    void initialize();
    void processMessage(const QString &message);
    void setNetworkManager(NetworkManager *manager);
    void setLearningModule(LearningModule *module);
    
    // Learning methods
    void learnFromInteraction(const QString &input, const QString &output);
    void updateKnowledgeBase(const QString &topic, const QString &information);
    QString generateResponse(const QString &input);
    
    // Code generation
    QString generateCode(const QString &description, const QString &language = "cpp");
    bool validateCode(const QString &code, const QString &language);
    
    // Context management
    void addToContext(const QString &message, const QString &response);
    void clearContext();
    QString getContextSummary();

signals:
    void responseReady(const QString &response);
    void learningProgressUpdated(int progress);
    void statusChanged(const QString &status);
    void codeGenerated(const QString &code);
    void errorOccurred(const QString &error);

private slots:
    void processNetworkResponse(const QString &response);
    void onLearningUpdate();

private:
    void initializeKnowledgeBase();
    void saveKnowledgeBase();
    void loadKnowledgeBase();
    
    QString analyzeInput(const QString &input);
    QString findBestResponse(const QString &input);
    double calculateConfidence(const QString &input, const QString &response);
    
    QStringList tokenize(const QString &text);
    QString preprocessText(const QString &text);
    
    NetworkManager *networkManager;
    LearningModule *learningModule;
    
    ConversationContext context;
    KnowledgeBase knowledgeBase;
    
    QTimer *learningTimer;
    QString currentQuery;
    bool isProcessing;
    
    // Neural network simulation (simplified)
    QVector<QVector<double>> weights;
    QVector<double> biases;
    int inputSize;
    int hiddenSize;
    int outputSize;
    
    void initializeNeuralNetwork();
    QVector<double> forwardPass(const QVector<double> &input);
    void backpropagate(const QVector<double> &input, const QVector<double> &target);
    double sigmoid(double x);
    double sigmoidDerivative(double x);
};

#endif // AIENGINE_H