#include "MainWindow.h"
#include "AIEngine.h"
#include "NetworkManager.h"
#include "CodeGenerator.h"
#include "LearningModule.h"

#include <QtWidgets/QApplication>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
#include <QtCore/QDateTime>
#include <QtCore/QStandardPaths>
#include <QtWidgets/QHeaderView>
#include <QtCore/QTextStream>
#include <QtGui/QFont>
#include <QtGui/QTextCursor>
#include <QtWidgets/QScrollBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , centralWidget(nullptr)
    , mainSplitter(nullptr)
    , tabWidget(nullptr)
    , chatTab(nullptr)
    , chatDisplay(nullptr)
    , messageInput(nullptr)
    , sendButton(nullptr)
    , clearButton(nullptr)
    , codeTab(nullptr)
    , codeEditor(nullptr)
    , generateButton(nullptr)
    , executeButton(nullptr)
    , codeOutput(nullptr)
    , learningTab(nullptr)
    , learningProgress(nullptr)
    , learningLog(nullptr)
    , learningStatus(nullptr)
    , connectionStatus(nullptr)
    , aiStatus(nullptr)
    , processingProgress(nullptr)
    , aiEngine(nullptr)
    , networkManager(nullptr)
    , codeGenerator(nullptr)
    , learningModule(nullptr)
    , statusTimer(nullptr)
{
    setWindowTitle("AI Assistant - Inteligentný Asistent");
    setMinimumSize(1200, 800);
    resize(1400, 900);
    
    // Initialize core components
    aiEngine = new AIEngine(this);
    networkManager = new NetworkManager(this);
    codeGenerator = new CodeGenerator(this);
    learningModule = new LearningModule(this);
    
    // Setup UI
    setupUI();
    setupMenuBar();
    setupStatusBar();
    connectSignals();
    
    // Initialize AI components
    aiEngine->setNetworkManager(networkManager);
    aiEngine->setLearningModule(learningModule);
    aiEngine->initialize();
    
    // Setup status timer
    statusTimer = new QTimer(this);
    connect(statusTimer, &QTimer::timeout, this, [this]() {
        networkManager->checkInternetConnection();
    });
    statusTimer->start(30000); // Check every 30 seconds
    
    // Initial status update
    updateStatus("Pripravený na komunikáciu");
    addMessageToChat("Systém", "AI Assistant je pripravený. Môžete začať konverzáciu!", "#00AA00");
}

MainWindow::~MainWindow()
{
    // Save current state before closing
    if (aiEngine) {
        // Save learning data and knowledge base
    }
}

void MainWindow::setupUI()
{
    centralWidget = new QWidget;
    setCentralWidget(centralWidget);
    
    // Create main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    
    // Create tab widget for different functions
    tabWidget = new QTabWidget;
    tabWidget->setTabPosition(QTabWidget::North);
    tabWidget->setMovable(true);
    
    // Setup chat tab
    setupChatTab();
    
    // Setup code tab
    setupCodeTab();
    
    // Setup learning tab
    setupLearningTab();
    
    mainLayout->addWidget(tabWidget);
}

void MainWindow::setupChatTab()
{
    chatTab = new QWidget;
    QVBoxLayout *chatLayout = new QVBoxLayout(chatTab);
    
    // Chat display area
    chatDisplay = new QTextEdit;
    chatDisplay->setReadOnly(true);
    chatDisplay->setFont(QFont("Consolas", 11));
    chatDisplay->setStyleSheet(
        "QTextEdit {"
        "    background-color: #1e1e1e;"
        "    color: #ffffff;"
        "    border: 1px solid #404040;"
        "    border-radius: 5px;"
        "    padding: 10px;"
        "}"
    );
    
    // Input area
    QHBoxLayout *inputLayout = new QHBoxLayout;
    
    messageInput = new QLineEdit;
    messageInput->setPlaceholderText("Napíšte svoju správu alebo otázku...");
    messageInput->setFont(QFont("Arial", 11));
    messageInput->setStyleSheet(
        "QLineEdit {"
        "    background-color: #2d2d2d;"
        "    color: #ffffff;"
        "    border: 2px solid #404040;"
        "    border-radius: 8px;"
        "    padding: 8px;"
        "    font-size: 11pt;"
        "}"
        "QLineEdit:focus {"
        "    border-color: #0078d4;"
        "}"
    );
    
    sendButton = new QPushButton("Odoslať");
    sendButton->setFont(QFont("Arial", 10, QFont::Bold));
    sendButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #0078d4;"
        "    color: white;"
        "    border: none;"
        "    border-radius: 8px;"
        "    padding: 8px 20px;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "    background-color: #106ebe;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #005a9e;"
        "}"
    );
    
    clearButton = new QPushButton("Vymazať");
    clearButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #d13438;"
        "    color: white;"
        "    border: none;"
        "    border-radius: 8px;"
        "    padding: 8px 20px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #b71c1c;"
        "}"
    );
    
    inputLayout->addWidget(messageInput);
    inputLayout->addWidget(sendButton);
    inputLayout->addWidget(clearButton);
    
    chatLayout->addWidget(chatDisplay, 1);
    chatLayout->addLayout(inputLayout);
    
    tabWidget->addTab(chatTab, "💬 Chat");
}

void MainWindow::setupCodeTab()
{
    codeTab = new QWidget;
    QVBoxLayout *codeLayout = new QVBoxLayout(codeTab);
    
    // Code editor
    codeEditor = new QTextEdit;
    codeEditor->setFont(QFont("Consolas", 11));
    codeEditor->setPlaceholderText("Sem bude generovaný kód alebo môžete písať vlastný...");
    codeEditor->setStyleSheet(
        "QTextEdit {"
        "    background-color: #1e1e1e;"
        "    color: #dcdcdc;"
        "    border: 1px solid #404040;"
        "    border-radius: 5px;"
        "    padding: 10px;"
        "    font-family: 'Consolas', monospace;"
        "}"
    );
    
    // Buttons
    QHBoxLayout *codeButtonLayout = new QHBoxLayout;
    
    generateButton = new QPushButton("🤖 Generovať kód");
    generateButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #16a085;"
        "    color: white;"
        "    border: none;"
        "    border-radius: 8px;"
        "    padding: 8px 16px;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "    background-color: #138d75;"
        "}"
    );
    
    executeButton = new QPushButton("▶️ Spustiť");
    executeButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #f39c12;"
        "    color: white;"
        "    border: none;"
        "    border-radius: 8px;"
        "    padding: 8px 16px;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "    background-color: #e67e22;"
        "}"
    );
    
    codeButtonLayout->addWidget(generateButton);
    codeButtonLayout->addWidget(executeButton);
    codeButtonLayout->addStretch();
    
    // Output area
    codeOutput = new QTextEdit;
    codeOutput->setReadOnly(true);
    codeOutput->setMaximumHeight(150);
    codeOutput->setFont(QFont("Consolas", 10));
    codeOutput->setStyleSheet(
        "QTextEdit {"
        "    background-color: #0c0c0c;"
        "    color: #00ff00;"
        "    border: 1px solid #404040;"
        "    border-radius: 5px;"
        "    padding: 8px;"
        "}"
    );
    
    codeLayout->addWidget(codeEditor, 2);
    codeLayout->addLayout(codeButtonLayout);
    codeLayout->addWidget(new QLabel("Výstup:"));
    codeLayout->addWidget(codeOutput, 1);
    
    tabWidget->addTab(codeTab, "💻 Kód");
}

void MainWindow::setupLearningTab()
{
    learningTab = new QWidget;
    QVBoxLayout *learningLayout = new QVBoxLayout(learningTab);
    
    // Learning status
    learningStatus = new QLabel("Stav učenia: Pripravený");
    learningStatus->setFont(QFont("Arial", 12, QFont::Bold));
    learningStatus->setStyleSheet("color: #00aa00; padding: 10px;");
    
    // Learning progress
    learningProgress = new QProgressBar;
    learningProgress->setStyleSheet(
        "QProgressBar {"
        "    border: 2px solid #404040;"
        "    border-radius: 8px;"
        "    text-align: center;"
        "    background-color: #2d2d2d;"
        "}"
        "QProgressBar::chunk {"
        "    background-color: #0078d4;"
        "    border-radius: 6px;"
        "}"
    );
    
    // Learning log
    learningLog = new QTextEdit;
    learningLog->setReadOnly(true);
    learningLog->setFont(QFont("Consolas", 10));
    learningLog->setStyleSheet(
        "QTextEdit {"
        "    background-color: #1e1e1e;"
        "    color: #ffffff;"
        "    border: 1px solid #404040;"
        "    border-radius: 5px;"
        "    padding: 10px;"
        "}"
    );
    
    learningLayout->addWidget(learningStatus);
    learningLayout->addWidget(learningProgress);
    learningLayout->addWidget(new QLabel("Log učenia:"));
    learningLayout->addWidget(learningLog);
    
    tabWidget->addTab(learningTab, "🧠 Učenie");
}

void MainWindow::setupMenuBar()
{
    QMenuBar *menuBar = this->menuBar();
    
    // File menu
    QMenu *fileMenu = menuBar->addMenu("Súbor");
    fileMenu->addAction("Uložiť konverzáciu", this, &MainWindow::saveConversation);
    fileMenu->addAction("Načítať konverzáciu", this, &MainWindow::loadConversation);
    fileMenu->addSeparator();
    fileMenu->addAction("Ukončiť", this, &QWidget::close);
    
    // AI menu
    QMenu *aiMenu = menuBar->addMenu("AI");
    aiMenu->addAction("Reštartovať AI", [this]() {
        aiEngine->initialize();
        updateStatus("AI reštartované");
    });
    aiMenu->addAction("Vymazať pamäť", [this]() {
        aiEngine->clearContext();
        addMessageToChat("Systém", "Pamäť AI vymazaná", "#FFA500");
    });
    
    // Help menu
    QMenu *helpMenu = menuBar->addMenu("Pomoc");
    helpMenu->addAction("O aplikácii", [this]() {
        QMessageBox::about(this, "O AI Assistentovi",
            "AI Assistant v1.0\n\n"
            "Inteligentný asistent s možnosťou učenia\n"
            "a generovania kódu.\n\n"
            "Vytvorené pomocou Qt6 a C++");
    });
}

void MainWindow::setupStatusBar()
{
    statusBar = this->statusBar();
    
    connectionStatus = new QLabel("🔴 Offline");
    aiStatus = new QLabel("AI: Pripravené");
    processingProgress = new QProgressBar;
    processingProgress->setVisible(false);
    processingProgress->setMaximumWidth(200);
    
    statusBar->addWidget(connectionStatus);
    statusBar->addPermanentWidget(aiStatus);
    statusBar->addPermanentWidget(processingProgress);
}

void MainWindow::connectSignals()
{
    // UI signals
    connect(sendButton, &QPushButton::clicked, this, &MainWindow::sendMessage);
    connect(clearButton, &QPushButton::clicked, this, &MainWindow::clearChat);
    connect(generateButton, &QPushButton::clicked, this, &MainWindow::generateCode);
    connect(executeButton, &QPushButton::clicked, this, &MainWindow::executeCode);
    connect(messageInput, &QLineEdit::returnPressed, this, &MainWindow::sendMessage);
    
    // AI Engine signals
    connect(aiEngine, &AIEngine::responseReady, this, &MainWindow::onAIResponse);
    connect(aiEngine, &AIEngine::statusChanged, this, &MainWindow::updateStatus);
    connect(aiEngine, &AIEngine::codeGenerated, [this](const QString &code) {
        codeEditor->setPlainText(code);
        tabWidget->setCurrentIndex(1); // Switch to code tab
    });
    
    // Network signals
    connect(networkManager, &NetworkManager::connectionStatusChanged, [this](bool connected) {
        connectionStatus->setText(connected ? "🟢 Online" : "🔴 Offline");
    });
    
    // Learning signals
    connect(learningModule, &LearningModule::learningProgressUpdated, 
            this, &MainWindow::onLearningProgress);
    connect(learningModule, &LearningModule::patternRecognized, [this](const QString &pattern) {
        learningLog->append(QString("[%1] Rozpoznaný vzor: %2")
                           .arg(QDateTime::currentDateTime().toString("hh:mm:ss"))
                           .arg(pattern));
    });
}

void MainWindow::sendMessage()
{
    QString message = messageInput->text().trimmed();
    if (message.isEmpty()) return;
    
    // Add user message to chat
    addMessageToChat("Používateľ", message, "#4CAF50");
    messageInput->clear();
    
    // Show processing
    processingProgress->setVisible(true);
    processingProgress->setRange(0, 0); // Indeterminate progress
    updateStatus("Spracúvam správu...");
    
    // Process with AI
    aiEngine->processMessage(message);
}

void MainWindow::clearChat()
{
    chatDisplay->clear();
    addMessageToChat("Systém", "Chat vymazaný", "#FFA500");
}

void MainWindow::saveConversation()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        "Uložiť konverzáciu", 
        QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/conversation.txt",
        "Text Files (*.txt)");
    
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out << chatDisplay->toPlainText();
            updateStatus("Konverzácia uložená");
        }
    }
}

void MainWindow::loadConversation()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        "Načítať konverzáciu",
        QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
        "Text Files (*.txt)");
    
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            chatDisplay->setPlainText(in.readAll());
            updateStatus("Konverzácia načítaná");
        }
    }
}

void MainWindow::onAIResponse(const QString &response)
{
    addMessageToChat("AI Assistant", response, "#2196F3");
    
    processingProgress->setVisible(false);
    updateStatus("Pripravený");
}

void MainWindow::onLearningProgress(int progress)
{
    learningProgress->setValue(progress);
    learningStatus->setText(QString("Stav učenia: %1%").arg(progress));
    
    if (progress == 100) {
        learningStatus->setText("Stav učenia: Dokončené ✓");
        learningLog->append(QString("[%1] Cyklus učenia dokončený")
                           .arg(QDateTime::currentDateTime().toString("hh:mm:ss")));
    }
}

void MainWindow::updateStatus(const QString &status)
{
    aiStatus->setText("AI: " + status);
    statusBar->showMessage(status, 3000);
}

void MainWindow::generateCode()
{
    QString description = messageInput->text().trimmed();
    if (description.isEmpty()) {
        description = "Vytvor jednoduchý C++ program";
    }
    
    updateStatus("Generujem kód...");
    processingProgress->setVisible(true);
    processingProgress->setRange(0, 0);
    
    // Generate code using AI
    QString code = aiEngine->generateCode(description);
    codeEditor->setPlainText(code);
    
    processingProgress->setVisible(false);
    updateStatus("Kód vygenerovaný");
}

void MainWindow::executeCode()
{
    QString code = codeEditor->toPlainText();
    if (code.isEmpty()) {
        codeOutput->setText("Chyba: Žiadny kód na spustenie!");
        return;
    }
    
    codeOutput->setText("Spúšťam kód...\n");
    codeOutput->append("Poznámka: Skutočné spustenie kódu vyžaduje kompilátor.");
    codeOutput->append("Pre C++ kód použite: g++ -o program program.cpp && ./program");
    
    // Here you could implement actual code execution
    // For safety, we just simulate it
    updateStatus("Kód pripravený na spustenie");
}

void MainWindow::addMessageToChat(const QString &sender, const QString &message, const QString &color)
{
    QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
    QString formattedMessage = QString(
        "<div style='margin: 8px 0; padding: 8px; background-color: rgba(255,255,255,0.05); border-radius: 8px;'>"
        "<span style='color: %1; font-weight: bold;'>[%2] %3:</span><br>"
        "<span style='color: #ffffff; margin-left: 20px;'>%4</span>"
        "</div>"
    ).arg(color, timestamp, sender, message.toHtmlEscaped());
    
    chatDisplay->insertHtml(formattedMessage);
    
    // Auto-scroll to bottom
    QTextCursor cursor = chatDisplay->textCursor();
    cursor.movePosition(QTextCursor::End);
    chatDisplay->setTextCursor(cursor);
    chatDisplay->ensureCursorVisible();
}

#include "MainWindow.moc"