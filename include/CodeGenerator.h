#ifndef CODEGENERATOR_H
#define CODEGENERATOR_H

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <functional>
#include <regex>

struct CodeTemplate {
    std::string language;
    std::string pattern;
    std::string template_code;
    std::vector<std::string> variables;
    std::string description;
};

struct GeneratedCode {
    std::string code;
    std::string language;
    std::string description;
    std::vector<std::string> dependencies;
    double confidence;
    bool is_valid;
};

class CodeGenerator
{
public:
    // Callback types
    using CodeCallback = std::function<void(const GeneratedCode&)>;
    using ValidationCallback = std::function<void(bool, const std::string&)>;
    using OptimizationCallback = std::function<void(const std::string&)>;
    using ProgressCallback = std::function<void(int)>;

    explicit CodeGenerator();
    ~CodeGenerator();
    
    // Callback setters
    void set_code_callback(CodeCallback callback);
    void set_validation_callback(ValidationCallback callback);
    void set_optimization_callback(OptimizationCallback callback);
    void set_progress_callback(ProgressCallback callback);
    
    // Code generation
    GeneratedCode generate_code(const std::string &description, const std::string &language = "cpp");
    std::string generate_function(const std::string &function_name, const std::string &description, 
                                 const std::vector<std::string> &parameters, const std::string &return_type);
    std::string generate_class(const std::string &class_name, const std::string &description,
                              const std::vector<std::string> &methods, const std::vector<std::string> &members);
    
    // Template management
    void add_template(const CodeTemplate &template_obj);
    void load_templates(const std::string &file_path);
    void save_templates(const std::string &file_path);
    
    // Code analysis
    bool validate_syntax(const std::string &code, const std::string &language);
    std::vector<std::string> extract_functions(const std::string &code);
    std::vector<std::string> extract_classes(const std::string &code);
    std::vector<std::string> find_dependencies(const std::string &code, const std::string &language);
    
    // Learning from code
    void learn_from_code(const std::string &code, const std::string &description);
    void analyze_code_patterns(const std::string &code);
    
    // Code optimization
    std::string optimize_code(const std::string &code, const std::string &language);
    std::string add_comments(const std::string &code, const std::string &language);
    std::string format_code(const std::string &code, const std::string &language);

private:
    void initialize_templates();
    void setup_language_patterns();
    
    CodeTemplate find_best_template(const std::string &description, const std::string &language);
    std::string fill_template(const CodeTemplate &template_obj, const std::string &description);
    std::map<std::string, std::string> extract_variables(const std::string &description);
    
    // Language-specific generators
    std::string generate_cpp_code(const std::string &description);
    std::string generate_python_code(const std::string &description);
    std::string generate_javascript_code(const std::string &description);
    std::string generate_java_code(const std::string &description);
    
    // Syntax validation
    bool validate_cpp_syntax(const std::string &code);
    bool validate_python_syntax(const std::string &code);
    bool validate_javascript_syntax(const std::string &code);
    
    // Pattern recognition
    std::vector<std::string> identify_patterns(const std::string &description);
    std::string match_pattern(const std::string &pattern, const std::string &description);
    
    std::vector<CodeTemplate> templates;
    std::map<std::string, std::vector<std::string>> language_keywords;
    std::map<std::string, std::regex> syntax_patterns;
    std::map<std::string, std::vector<std::string>> common_patterns;
    
    // Learning data
    std::map<std::string, std::vector<std::string>> learned_patterns;
    std::map<std::string, std::string> code_examples;
    std::map<std::string, double> pattern_confidence;
    
    // Callbacks
    CodeCallback code_callback;
    ValidationCallback validation_callback;
    OptimizationCallback optimization_callback;
    ProgressCallback progress_callback;
    
    int learning_progress;
};

#endif // CODEGENERATOR_H