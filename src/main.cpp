#include <gtkmm.h>
#include <iostream>
#include <memory>
#include <filesystem>
#include <curl/curl.h>

#include "MainWindow.h"

class AIAssistantApplication : public Gtk::Application
{
public:
    AIAssistantApplication() : Gtk::Application("org.aiassistant.app") {}

protected:
    void on_activate() override {
        try {
            auto window = std::make_unique<MainWindow>();
            add_window(*window);
            
            // Initialize the window
            window->initialize();
            window->present();
            
            // Keep window alive
            m_window = std::move(window);
            
        } catch (const std::exception& e) {
            std::cerr << "Chyba pri vytváraní okna: " << e.what() << std::endl;
            quit();
        }
    }

    void on_startup() override {
        Gtk::Application::on_startup();
        
        // Setup application-wide styling
        setup_styling();
        
        // Create data directories
        create_data_directories();
        
        std::cout << "AI Assistant - Inteligentný Asistent v1.0" << std::endl;
        std::cout << "Inicializujem aplikáciu..." << std::endl;
    }

    void on_shutdown() override {
        std::cout << "Ukončujem AI Assistant..." << std::endl;
        Gtk::Application::on_shutdown();
    }

private:
    void setup_styling() {
        auto css_provider = Gtk::CssProvider::create();
        
        // Dark theme CSS
        std::string css_data = R"(
            window {
                background-color: #2b2b2b;
                color: #ffffff;
            }
            
            .dark-theme {
                background-color: #1e1e1e;
                color: #ffffff;
            }
            
            .chat-display {
                background-color: #1e1e1e;
                color: #ffffff;
                font-family: 'Consolas', 'DejaVu Sans Mono', monospace;
                font-size: 11pt;
                padding: 10px;
            }
            
            .code-editor {
                background-color: #1e1e1e;
                color: #dcdcdc;
                font-family: 'Consolas', 'DejaVu Sans Mono', monospace;
                font-size: 11pt;
                padding: 10px;
            }
            
            .code-output {
                background-color: #0c0c0c;
                color: #00ff00;
                font-family: 'Consolas', 'DejaVu Sans Mono', monospace;
                font-size: 10pt;
                padding: 8px;
            }
            
            .message-entry {
                background-color: #2d2d2d;
                color: #ffffff;
                border: 2px solid #404040;
                border-radius: 8px;
                padding: 8px;
                font-size: 11pt;
            }
            
            .message-entry:focus {
                border-color: #0078d4;
            }
            
            .send-button {
                background: linear-gradient(to bottom, #0078d4, #106ebe);
                color: white;
                border: none;
                border-radius: 8px;
                padding: 8px 20px;
                font-weight: bold;
            }
            
            .send-button:hover {
                background: linear-gradient(to bottom, #106ebe, #005a9e);
            }
            
            .clear-button {
                background: linear-gradient(to bottom, #d13438, #b71c1c);
                color: white;
                border: none;
                border-radius: 8px;
                padding: 8px 20px;
            }
            
            .generate-button {
                background: linear-gradient(to bottom, #16a085, #138d75);
                color: white;
                border: none;
                border-radius: 8px;
                padding: 8px 16px;
                font-weight: bold;
            }
            
            .execute-button {
                background: linear-gradient(to bottom, #f39c12, #e67e22);
                color: white;
                border: none;
                border-radius: 8px;
                padding: 8px 16px;
                font-weight: bold;
            }
            
            .status-label {
                color: #00aa00;
                padding: 10px;
                font-weight: bold;
            }
            
            progressbar {
                border: 2px solid #404040;
                border-radius: 8px;
                background-color: #2d2d2d;
            }
            
            progressbar progress {
                background-color: #0078d4;
                border-radius: 6px;
            }
            
            notebook {
                background-color: #2b2b2b;
            }
            
            notebook tab {
                background-color: #404040;
                color: #ffffff;
                padding: 8px 16px;
                border-radius: 8px 8px 0 0;
            }
            
            notebook tab:checked {
                background-color: #0078d4;
            }
        )";
        
        try {
            css_provider->load_from_data(css_data);
            
            auto display = Gdk::Display::get_default();
            Gtk::StyleContext::add_provider_for_display(
                display, css_provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
                
        } catch (const std::exception& e) {
            std::cerr << "Chyba pri načítavaní CSS: " << e.what() << std::endl;
        }
    }
    
    void create_data_directories() {
        try {
            std::filesystem::path home_dir = std::filesystem::path(std::getenv("HOME"));
            std::filesystem::path app_data_dir = home_dir / ".local" / "share" / "AI_Assistant";
            
            std::filesystem::create_directories(app_data_dir);
            std::filesystem::create_directories(app_data_dir / "knowledge");
            std::filesystem::create_directories(app_data_dir / "conversations");
            std::filesystem::create_directories(app_data_dir / "generated_code");
            std::filesystem::create_directories(app_data_dir / "learning_data");
            
            std::cout << "Dátové adresáre vytvorené v: " << app_data_dir << std::endl;
            
        } catch (const std::exception& e) {
            std::cerr << "Chyba pri vytváraní adresárov: " << e.what() << std::endl;
        }
    }
    
    std::unique_ptr<MainWindow> m_window;
};

void show_welcome_message() {
    std::cout << "\n=== AI Assistant - Vitajte! ===" << std::endl;
    std::cout << "Tento asistent dokáže:" << std::endl;
    std::cout << "• Komunikovať s vami v prirodzenom jazyku" << std::endl;
    std::cout << "• Generovať kód v rôznych jazykoch" << std::endl;
    std::cout << "• Učiť sa z vašich interakcií" << std::endl;
    std::cout << "• Pristupovať na internet pre aktuálne informácie" << std::endl;
    std::cout << "\nZačnite písaním správy v chat okne." << std::endl;
    std::cout << "================================\n" << std::endl;
}

int main(int argc, char* argv[])
{
    try {
        // Initialize CURL globally
        curl_global_init(CURL_GLOBAL_DEFAULT);
        
        // Create and run GTK application
        auto app = AIAssistantApplication::create();
        
        // Show welcome message in console
        show_welcome_message();
        
        // Run the application
        int result = app->run(argc, argv);
        
        // Cleanup CURL
        curl_global_cleanup();
        
        return result;
        
    } catch (const std::exception& e) {
        std::cerr << "Kritická chyba: " << e.what() << std::endl;
        return -1;
    } catch (...) {
        std::cerr << "Neznáma chyba pri spúšťaní aplikácie." << std::endl;
        return -1;
    }
}