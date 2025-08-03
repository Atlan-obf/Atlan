#include <QtWidgets/QApplication>
#include <QtCore/QDir>
#include <QtCore/QStandardPaths>
#include <QtCore/QLoggingCategory>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QStyleFactory>
#include <QtGui/QPalette>
#include <QtCore/QTranslator>

#include "MainWindow.h"

void setupLogging()
{
    QLoggingCategory::setFilterRules("qt.network.ssl.debug=false");
}

void setupApplicationStyle(QApplication &app)
{
    // Set modern dark theme
    app.setStyle(QStyleFactory::create("Fusion"));
    
    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(25, 25, 25));
    darkPalette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);
    
    app.setPalette(darkPalette);
}

void createDataDirectories()
{
    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dataDir;
    
    if (!dataDir.exists(appDataPath)) {
        dataDir.mkpath(appDataPath);
    }
    
    // Create subdirectories for different data types
    dataDir.mkpath(appDataPath + "/knowledge");
    dataDir.mkpath(appDataPath + "/conversations");
    dataDir.mkpath(appDataPath + "/generated_code");
    dataDir.mkpath(appDataPath + "/learning_data");
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // Set application properties
    app.setApplicationName("AI Assistant");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("AI Development Team");
    app.setApplicationDisplayName("AI Assistant - Inteligentný Asistent");
    
    // Setup logging and styling
    setupLogging();
    setupApplicationStyle(app);
    
    // Create necessary directories
    createDataDirectories();
    
    // Setup translation (Slovak support)
    QTranslator translator;
    if (translator.load("aiassistant_sk", ":/translations/")) {
        app.installTranslator(&translator);
    }
    
    try {
        // Create and show main window
        MainWindow window;
        window.show();
        
        // Show welcome message
        QMessageBox::information(&window, 
            "AI Assistant", 
            "Vitajte v AI Assistentovi!\n\n"
            "Tento asistent dokáže:\n"
            "• Komunikovať s vami v prirodzenom jazyku\n"
            "• Generovať kód v rôznych jazykoch\n"
            "• Učiť sa z vašich interakcií\n"
            "• Pristupovať na internet pre aktuálne informácie\n\n"
            "Začnite písaním správy v chat okne.");
        
        return app.exec();
    }
    catch (const std::exception &e) {
        QMessageBox::critical(nullptr, 
            "Kritická chyba", 
            QString("Aplikácia sa nemôže spustiť:\n%1").arg(e.what()));
        return -1;
    }
    catch (...) {
        QMessageBox::critical(nullptr, 
            "Neznáma chyba", 
            "Nastala neočakávaná chyba pri spúšťaní aplikácie.");
        return -1;
    }
}