#ifndef AIENGINE_H
#define AIENGINE_H

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <functional>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>

class NetworkManager;
class LearningModule;

struct ConversationContext {
    std::vector<std::string> messages;
    std::vector<std::string> responses;
    std::string current_topic;
    int context_length;
};

struct KnowledgeBase {
    std::map<std::string, std::string> facts;
    std::map<std::string, std::vector<std::string>> patterns;
    std::map<std::string, double> confidence;
    std::vector<std::string> code_examples;
};

class AIEngine
{
public:
    // Callback types
    using ResponseCallback = std::function<void(const std::string&)>;
    using ProgressCallback = std::function<void(int)>;
    using StatusCallback = std::function<void(const std::string&)>;
    using CodeCallback = std::function<void(const std::string&)>;
    using ErrorCallback = std::function<void(const std::string&)>;

    explicit AIEngine();
    ~AIEngine();
    
    void initialize();
    void process_message(const std::string &message);
    void set_network_manager(std::shared_ptr<NetworkManager> manager);
    void set_learning_module(std::shared_ptr<LearningModule> module);
    
    // Callback setters
    void set_response_callback(ResponseCallback callback);
    void set_progress_callback(ProgressCallback callback);
    void set_status_callback(StatusCallback callback);
    void set_code_callback(CodeCallback callback);
    void set_error_callback(ErrorCallback callback);
    
    // Learning methods
    void learn_from_interaction(const std::string &input, const std::string &output);
    void update_knowledge_base(const std::string &topic, const std::string &information);
    std::string generate_response(const std::string &input);
    
    // Code generation
    std::string generate_code(const std::string &description, const std::string &language = "cpp");
    bool validate_code(const std::string &code, const std::string &language);
    
    // Context management
    void add_to_context(const std::string &message, const std::string &response);
    void clear_context();
    std::string get_context_summary();

private:
    void process_network_response(const std::string &response);
    void on_learning_update();
    
    void initialize_knowledge_base();
    void save_knowledge_base();
    void load_knowledge_base();
    
    std::string analyze_input(const std::string &input);
    std::string find_best_response(const std::string &input);
    double calculate_confidence(const std::string &input, const std::string &response);
    
    std::vector<std::string> tokenize(const std::string &text);
    std::string preprocess_text(const std::string &text);
    
    std::shared_ptr<NetworkManager> network_manager;
    std::shared_ptr<LearningModule> learning_module;
    
    ConversationContext context;
    KnowledgeBase knowledge_base;
    
    std::string current_query;
    std::atomic<bool> is_processing{false};
    
    // Callbacks
    ResponseCallback response_callback;
    ProgressCallback progress_callback;
    StatusCallback status_callback;
    CodeCallback code_callback;
    ErrorCallback error_callback;
    
    // Threading
    std::mutex processing_mutex;
    std::thread learning_thread;
    std::atomic<bool> should_stop{false};
    
    // Neural network simulation (simplified)
    std::vector<std::vector<double>> weights;
    std::vector<double> biases;
    int input_size;
    int hidden_size;
    int output_size;
    
    void initialize_neural_network();
    std::vector<double> forward_pass(const std::vector<double> &input);
    void backpropagate(const std::vector<double> &input, const std::vector<double> &target);
    double sigmoid(double x);
    double sigmoid_derivative(double x);
    
    // Helper methods
    void emit_response(const std::string &response);
    void emit_progress(int progress);
    void emit_status(const std::string &status);
    void emit_code(const std::string &code);
    void emit_error(const std::string &error);
};

#endif // AIENGINE_H