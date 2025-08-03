#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <gtkmm.h>
#include <memory>
#include <thread>
#include <mutex>
#include <atomic>

class AIEngine;
class NetworkManager;
class CodeGenerator;
class LearningModule;

class MainWindow : public Gtk::ApplicationWindow
{
public:
    MainWindow();
    virtual ~MainWindow();

    void initialize();

protected:
    // Signal handlers
    void on_send_button_clicked();
    void on_clear_button_clicked();
    void on_generate_code_clicked();
    void on_execute_code_clicked();
    void on_message_entry_activate();
    void on_notebook_switch_page(Gtk::Widget* page, guint page_num);
    
    // Menu actions
    void on_menu_save_conversation();
    void on_menu_load_conversation();
    void on_menu_restart_ai();
    void on_menu_clear_memory();
    void on_menu_about();
    void on_menu_quit();

private:
    void setup_ui();
    void setup_menu();
    void setup_chat_tab();
    void setup_code_tab();
    void setup_learning_tab();
    void setup_status_bar();
    void connect_signals();
    
    void add_message_to_chat(const std::string &sender, const std::string &message, const std::string &color = "#FFFFFF");
    void update_status(const std::string &status);
    void update_connection_status(bool connected);
    void update_learning_progress(int progress);
    void show_processing(bool show);
    
    // AI response handling (thread-safe)
    void on_ai_response_ready(const std::string &response);
    void on_code_generated(const std::string &code);
    void on_learning_progress_updated(int progress);
    
    // Thread-safe UI updates
    void queue_ui_update(std::function<void()> update_func);
    
    // Main container
    Gtk::Box main_box;
    Gtk::MenuBar menu_bar;
    Gtk::Notebook notebook;
    Gtk::Box status_box;
    
    // Chat tab widgets
    Gtk::Box chat_box;
    Gtk::ScrolledWindow chat_scrolled;
    Gtk::TextView chat_display;
    Glib::RefPtr<Gtk::TextBuffer> chat_buffer;
    Gtk::Box input_box;
    Gtk::Entry message_entry;
    Gtk::Button send_button;
    Gtk::Button clear_button;
    
    // Code tab widgets
    Gtk::Box code_box;
    Gtk::ScrolledWindow code_scrolled;
    Gtk::TextView code_editor;
    Glib::RefPtr<Gtk::TextBuffer> code_buffer;
    Gtk::Box code_button_box;
    Gtk::Button generate_button;
    Gtk::Button execute_button;
    Gtk::ScrolledWindow output_scrolled;
    Gtk::TextView code_output;
    Glib::RefPtr<Gtk::TextBuffer> output_buffer;
    
    // Learning tab widgets
    Gtk::Box learning_box;
    Gtk::Label learning_status;
    Gtk::ProgressBar learning_progress;
    Gtk::ScrolledWindow learning_scrolled;
    Gtk::TextView learning_log;
    Glib::RefPtr<Gtk::TextBuffer> learning_buffer;
    
    // Status bar widgets
    Gtk::Label connection_status;
    Gtk::Label ai_status;
    Gtk::ProgressBar processing_progress;
    Gtk::Spinner processing_spinner;
    
    // Menu items
    Glib::RefPtr<Gio::SimpleActionGroup> action_group;
    
    // Core components
    std::unique_ptr<AIEngine> ai_engine;
    std::unique_ptr<NetworkManager> network_manager;
    std::unique_ptr<CodeGenerator> code_generator;
    std::unique_ptr<LearningModule> learning_module;
    
    // Threading
    std::mutex ui_mutex;
    std::atomic<bool> is_processing{false};
    
    // Timers
    sigc::connection status_timer;
    sigc::connection learning_timer;
};

#endif // MAINWINDOW_H