#ifndef LEARNINGMODULE_H
#define LEARNINGMODULE_H

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <functional>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>
#include <json/json.h>

struct LearningData {
    std::string input;
    std::string output;
    std::string context;
    double reward;
    int64_t timestamp;
    std::string category;
};

struct NeuralConnection {
    int from_node;
    int to_node;
    double weight;
    double last_update;
};

class LearningModule
{
public:
    // Callback types
    using ProgressCallback = std::function<void(int)>;
    using PatternCallback = std::function<void(const std::string&)>;
    using ConfidenceCallback = std::function<void(double)>;
    using KnowledgeCallback = std::function<void(const std::string&)>;
    using CompletionCallback = std::function<void()>;
    using ErrorCallback = std::function<void(const std::string&)>;

    explicit LearningModule();
    ~LearningModule();
    
    // Callback setters
    void set_progress_callback(ProgressCallback callback);
    void set_pattern_callback(PatternCallback callback);
    void set_confidence_callback(ConfidenceCallback callback);
    void set_knowledge_callback(KnowledgeCallback callback);
    void set_completion_callback(CompletionCallback callback);
    void set_error_callback(ErrorCallback callback);
    
    // Learning methods
    void learn(const std::string &input, const std::string &output, double reward = 1.0);
    void reinforcement_learning(const std::string &action, double reward);
    void unsupervised_learning(const std::vector<std::string> &data);
    
    // Pattern recognition
    std::vector<std::string> recognize_patterns(const std::string &input);
    std::string predict_output(const std::string &input);
    double calculate_confidence(const std::string &input, const std::string &output);
    
    // Knowledge management
    void save_knowledge(const std::string &file_path);
    void load_knowledge(const std::string &file_path);
    void update_knowledge(const std::string &key, const Json::Value &data);
    Json::Value get_knowledge(const std::string &key);
    
    // Neural network operations
    void initialize_network(int input_size, int hidden_size, int output_size);
    std::vector<double> process_input(const std::vector<double> &input);
    void train_network(const std::vector<std::vector<double>> &inputs, 
                      const std::vector<std::vector<double>> &targets);
    
    // Self-improvement
    void analyze_mistakes();
    void optimize_performance();
    void adapt_to_new_patterns();
    
    // Statistics and monitoring
    int get_total_learning_events() const;
    double get_average_confidence() const;
    std::vector<std::string> get_most_learned_patterns() const;
    std::string get_learning_report() const;

private:
    void initialize_learning_system();
    void process_learning_data();
    void update_neural_connections();
    
    // Pattern analysis
    std::vector<std::string> extract_features(const std::string &input);
    std::string find_similar_patterns(const std::string &input);
    void cluster_data();
    
    // Neural network helpers
    double activation_function(double x);
    double activation_derivative(double x);
    void backpropagate(const std::vector<double> &input, const std::vector<double> &target);
    void update_weights(double learning_rate);
    
    // Threading functions
    void continuous_learning();
    void evaluate_performance();
    
    // Helper methods
    std::string analyze_category(const std::string &input);
    
    // Data structures
    std::vector<LearningData> learning_history;
    std::vector<NeuralConnection> connections;
    std::map<std::string, Json::Value> knowledge_base;
    std::map<std::string, double> pattern_confidence;
    std::map<std::string, int> pattern_frequency;
    
    // Neural network
    std::vector<std::vector<double>> weights;
    std::vector<double> biases;
    std::vector<double> last_output;
    std::vector<double> last_hidden;
    
    int input_size;
    int hidden_size;
    int output_size;
    double learning_rate;
    double momentum;
    
    // Learning parameters
    int total_learning_events;
    double average_confidence;
    int max_history_size;
    
    std::atomic<bool> is_learning{false};
    bool adaptive_mode;
    std::string current_category;
    
    // Callbacks
    ProgressCallback progress_callback;
    PatternCallback pattern_callback;
    ConfidenceCallback confidence_callback;
    KnowledgeCallback knowledge_callback;
    CompletionCallback completion_callback;
    ErrorCallback error_callback;
    
    // Threading
    std::thread learning_thread;
    std::thread evaluation_thread;
    std::mutex learning_mutex;
    std::atomic<bool> should_stop{false};
};

#endif // LEARNINGMODULE_H