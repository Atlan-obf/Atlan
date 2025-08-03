#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QScrollArea>
#include <QtCore/QTimer>

class AIEngine;
class NetworkManager;
class CodeGenerator;
class LearningModule;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void sendMessage();
    void clearChat();
    void saveConversation();
    void loadConversation();
    void onAIResponse(const QString &response);
    void onLearningProgress(int progress);
    void updateStatus(const QString &status);
    void generateCode();
    void executeCode();

private:
    void setupUI();
    void setupMenuBar();
    void setupStatusBar();
    void connectSignals();
    void addMessageToChat(const QString &sender, const QString &message, const QString &color = "#FFFFFF");

    // UI Components
    QWidget *centralWidget;
    QSplitter *mainSplitter;
    QTabWidget *tabWidget;
    
    // Chat tab
    QWidget *chatTab;
    QTextEdit *chatDisplay;
    QLineEdit *messageInput;
    QPushButton *sendButton;
    QPushButton *clearButton;
    
    // Code tab
    QWidget *codeTab;
    QTextEdit *codeEditor;
    QPushButton *generateButton;
    QPushButton *executeButton;
    QTextEdit *codeOutput;
    
    // Learning tab
    QWidget *learningTab;
    QProgressBar *learningProgress;
    QTextEdit *learningLog;
    QLabel *learningStatus;
    
    // Status and menu
    QStatusBar *statusBar;
    QLabel *connectionStatus;
    QLabel *aiStatus;
    QProgressBar *processingProgress;
    
    // Core components
    AIEngine *aiEngine;
    NetworkManager *networkManager;
    CodeGenerator *codeGenerator;
    LearningModule *learningModule;
    
    QTimer *statusTimer;
};

#endif // MAINWINDOW_H