#ifndef CODEGENERATOR_H
#define CODEGENERATOR_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QMap>
#include <QtCore/QVector>
#include <QtCore/QRegularExpression>

struct CodeTemplate {
    QString language;
    QString pattern;
    QString template_code;
    QStringList variables;
    QString description;
};

struct GeneratedCode {
    QString code;
    QString language;
    QString description;
    QStringList dependencies;
    double confidence;
    bool isValid;
};

class CodeGenerator : public QObject
{
    Q_OBJECT

public:
    explicit CodeGenerator(QObject *parent = nullptr);
    ~CodeGenerator();
    
    // Code generation
    GeneratedCode generateCode(const QString &description, const QString &language = "cpp");
    QString generateFunction(const QString &functionName, const QString &description, 
                           const QStringList &parameters, const QString &returnType);
    QString generateClass(const QString &className, const QString &description,
                         const QStringList &methods, const QStringList &members);
    
    // Template management
    void addTemplate(const CodeTemplate &template_obj);
    void loadTemplates(const QString &filePath);
    void saveTemplates(const QString &filePath);
    
    // Code analysis
    bool validateSyntax(const QString &code, const QString &language);
    QStringList extractFunctions(const QString &code);
    QStringList extractClasses(const QString &code);
    QStringList findDependencies(const QString &code, const QString &language);
    
    // Learning from code
    void learnFromCode(const QString &code, const QString &description);
    void analyzeCodePatterns(const QString &code);
    
    // Code optimization
    QString optimizeCode(const QString &code, const QString &language);
    QString addComments(const QString &code, const QString &language);
    QString formatCode(const QString &code, const QString &language);

signals:
    void codeGenerated(const GeneratedCode &code);
    void validationComplete(bool isValid, const QString &errors);
    void optimizationComplete(const QString &optimizedCode);
    void learningProgressUpdated(int progress);

private:
    void initializeTemplates();
    void setupLanguagePatterns();
    
    CodeTemplate findBestTemplate(const QString &description, const QString &language);
    QString fillTemplate(const CodeTemplate &template_obj, const QString &description);
    QMap<QString, QString> extractVariables(const QString &description);
    
    // Language-specific generators
    QString generateCppCode(const QString &description);
    QString generatePythonCode(const QString &description);
    QString generateJavaScriptCode(const QString &description);
    QString generateJavaCode(const QString &description);
    
    // Syntax validation
    bool validateCppSyntax(const QString &code);
    bool validatePythonSyntax(const QString &code);
    bool validateJavaScriptSyntax(const QString &code);
    
    // Pattern recognition
    QStringList identifyPatterns(const QString &description);
    QString matchPattern(const QString &pattern, const QString &description);
    
    QVector<CodeTemplate> templates;
    QMap<QString, QStringList> languageKeywords;
    QMap<QString, QRegularExpression> syntaxPatterns;
    QMap<QString, QStringList> commonPatterns;
    
    // Learning data
    QMap<QString, QStringList> learnedPatterns;
    QMap<QString, QString> codeExamples;
    QMap<QString, double> patternConfidence;
    
    int learningProgress;
};

#endif // CODEGENERATOR_H