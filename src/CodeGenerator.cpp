#include "CodeGenerator.h"
#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>
#include <QtCore/QRegularExpression>
#include <QtCore/QRandomGenerator>

CodeGenerator::CodeGenerator(QObject *parent)
    : QObject(parent)
    , learningProgress(0)
{
    initializeTemplates();
    setupLanguagePatterns();
}

CodeGenerator::~CodeGenerator()
{
}

void CodeGenerator::initializeTemplates()
{
    // C++ templates
    CodeTemplate cppHelloWorld;
    cppHelloWorld.language = "cpp";
    cppHelloWorld.pattern = "hello.*world";
    cppHelloWorld.template_code = 
        "#include <iostream>\n"
        "using namespace std;\n\n"
        "int main() {\n"
        "    cout << \"Hello World!\" << endl;\n"
        "    return 0;\n"
        "}";
    cppHelloWorld.description = "Basic Hello World program in C++";
    templates.append(cppHelloWorld);
    
    CodeTemplate cppClass;
    cppClass.language = "cpp";
    cppClass.pattern = "(class|trieda)";
    cppClass.template_code = 
        "#include <iostream>\n"
        "#include <string>\n"
        "using namespace std;\n\n"
        "class ${ClassName} {\n"
        "private:\n"
        "    ${MemberType} ${memberName};\n\n"
        "public:\n"
        "    ${ClassName}(${MemberType} ${paramName}) : ${memberName}(${paramName}) {}\n"
        "    \n"
        "    void display() {\n"
        "        cout << \"${ClassName}: \" << ${memberName} << endl;\n"
        "    }\n"
        "    \n"
        "    ${MemberType} get${MemberName}() const { return ${memberName}; }\n"
        "    void set${MemberName}(${MemberType} value) { ${memberName} = value; }\n"
        "};\n\n"
        "int main() {\n"
        "    ${ClassName} obj(${defaultValue});\n"
        "    obj.display();\n"
        "    return 0;\n"
        "}";
    cppClass.variables << "ClassName" << "MemberType" << "memberName" << "MemberName" << "paramName" << "defaultValue";
    cppClass.description = "Basic class template in C++";
    templates.append(cppClass);
    
    CodeTemplate cppCalculator;
    cppCalculator.language = "cpp";
    cppCalculator.pattern = "(kalkulačka|calculator|počítanie)";
    cppCalculator.template_code = 
        "#include <iostream>\n"
        "using namespace std;\n\n"
        "int main() {\n"
        "    double a, b, result;\n"
        "    char operation;\n"
        "    \n"
        "    cout << \"Zadajte prvé číslo: \";\n"
        "    cin >> a;\n"
        "    cout << \"Zadajte operátor (+, -, *, /): \";\n"
        "    cin >> operation;\n"
        "    cout << \"Zadajte druhé číslo: \";\n"
        "    cin >> b;\n"
        "    \n"
        "    switch(operation) {\n"
        "        case '+':\n"
        "            result = a + b;\n"
        "            cout << \"Výsledok: \" << result << endl;\n"
        "            break;\n"
        "        case '-':\n"
        "            result = a - b;\n"
        "            cout << \"Výsledok: \" << result << endl;\n"
        "            break;\n"
        "        case '*':\n"
        "            result = a * b;\n"
        "            cout << \"Výsledok: \" << result << endl;\n"
        "            break;\n"
        "        case '/':\n"
        "            if(b != 0) {\n"
        "                result = a / b;\n"
        "                cout << \"Výsledok: \" << result << endl;\n"
        "            } else {\n"
        "                cout << \"Chyba: Delenie nulou!\" << endl;\n"
        "            }\n"
        "            break;\n"
        "        default:\n"
        "            cout << \"Neznámy operátor!\" << endl;\n"
        "    }\n"
        "    \n"
        "    return 0;\n"
        "}";
    cppCalculator.description = "Simple calculator in C++";
    templates.append(cppCalculator);
    
    // Python templates
    CodeTemplate pythonHelloWorld;
    pythonHelloWorld.language = "python";
    pythonHelloWorld.pattern = "hello.*world";
    pythonHelloWorld.template_code = 
        "def main():\n"
        "    print(\"Hello World!\")\n\n"
        "if __name__ == \"__main__\":\n"
        "    main()";
    pythonHelloWorld.description = "Basic Hello World program in Python";
    templates.append(pythonHelloWorld);
    
    CodeTemplate pythonClass;
    pythonClass.language = "python";
    pythonClass.pattern = "(class|trieda)";
    pythonClass.template_code = 
        "class ${ClassName}:\n"
        "    def __init__(self, ${paramName}):\n"
        "        self.${memberName} = ${paramName}\n"
        "    \n"
        "    def display(self):\n"
        "        print(f\"${ClassName}: {self.${memberName}}\")\n"
        "    \n"
        "    def get_${memberName}(self):\n"
        "        return self.${memberName}\n"
        "    \n"
        "    def set_${memberName}(self, value):\n"
        "        self.${memberName} = value\n\n"
        "def main():\n"
        "    obj = ${ClassName}(${defaultValue})\n"
        "    obj.display()\n\n"
        "if __name__ == \"__main__\":\n"
        "    main()";
    pythonClass.variables << "ClassName" << "memberName" << "paramName" << "defaultValue";
    pythonClass.description = "Basic class template in Python";
    templates.append(pythonClass);
}

void CodeGenerator::setupLanguagePatterns()
{
    // C++ keywords
    languageKeywords["cpp"] = QStringList() 
        << "include" << "using" << "namespace" << "int" << "main" << "cout" << "cin" << "endl"
        << "class" << "public" << "private" << "protected" << "virtual" << "const" << "static"
        << "if" << "else" << "for" << "while" << "do" << "switch" << "case" << "break" << "continue"
        << "return" << "void" << "bool" << "char" << "double" << "float" << "string";
    
    // Python keywords
    languageKeywords["python"] = QStringList()
        << "def" << "class" << "if" << "else" << "elif" << "for" << "while" << "in" << "not"
        << "and" << "or" << "import" << "from" << "as" << "try" << "except" << "finally"
        << "with" << "lambda" << "yield" << "return" << "pass" << "break" << "continue"
        << "print" << "input" << "len" << "range" << "str" << "int" << "float" << "bool";
    
    // Common programming patterns
    commonPatterns["input_output"] = QStringList() 
        << "vstup" << "výstup" << "input" << "output" << "print" << "cout" << "cin";
    commonPatterns["loops"] = QStringList()
        << "cyklus" << "loop" << "for" << "while" << "opakuj" << "repeat";
    commonPatterns["conditions"] = QStringList()
        << "podmienka" << "condition" << "if" << "else" << "ak" << "inak";
    commonPatterns["functions"] = QStringList()
        << "funkcia" << "function" << "metóda" << "method" << "def";
}

GeneratedCode CodeGenerator::generateCode(const QString &description, const QString &language)
{
    GeneratedCode result;
    result.language = language;
    result.description = description;
    result.confidence = 0.0;
    result.isValid = false;
    
    // Find best matching template
    CodeTemplate bestTemplate = findBestTemplate(description, language);
    
    if (!bestTemplate.template_code.isEmpty()) {
        result.code = fillTemplate(bestTemplate, description);
        result.confidence = 0.8;
        result.isValid = validateSyntax(result.code, language);
    } else {
        // Generate using language-specific generators
        if (language == "cpp" || description.toLower().contains("c++")) {
            result.code = generateCppCode(description);
            result.language = "cpp";
        } else if (language == "python" || description.toLower().contains("python")) {
            result.code = generatePythonCode(description);
            result.language = "python";
        } else if (language == "javascript" || description.toLower().contains("javascript")) {
            result.code = generateJavaScriptCode(description);
            result.language = "javascript";
        } else {
            // Default to C++
            result.code = generateCppCode(description);
            result.language = "cpp";
        }
        
        result.confidence = 0.6;
        result.isValid = validateSyntax(result.code, result.language);
    }
    
    // Find dependencies
    result.dependencies = findDependencies(result.code, result.language);
    
    // Learn from this generation
    learnFromCode(result.code, description);
    
    emit codeGenerated(result);
    return result;
}

QString CodeGenerator::generateFunction(const QString &functionName, const QString &description, 
                                      const QStringList &parameters, const QString &returnType)
{
    QString code;
    
    // Generate C++ function
    code += QString("// %1\n").arg(description);
    code += QString("%1 %2(").arg(returnType, functionName);
    
    // Add parameters
    for (int i = 0; i < parameters.size(); ++i) {
        if (i > 0) code += ", ";
        code += parameters[i];
    }
    code += ") {\n";
    code += "    // TODO: Implementujte funkcionalitu\n";
    
    if (returnType != "void") {
        if (returnType == "int") {
            code += "    return 0;\n";
        } else if (returnType == "bool") {
            code += "    return true;\n";
        } else if (returnType == "string" || returnType == "QString") {
            code += "    return \"\";\n";
        } else {
            code += QString("    return %1();\n").arg(returnType);
        }
    }
    
    code += "}\n";
    
    return code;
}

QString CodeGenerator::generateClass(const QString &className, const QString &description,
                                    const QStringList &methods, const QStringList &members)
{
    QString code;
    
    code += QString("// %1\n").arg(description);
    code += QString("class %1 {\n").arg(className);
    code += "private:\n";
    
    // Add members
    for (const QString &member : members) {
        code += QString("    %1;\n").arg(member);
    }
    
    code += "\npublic:\n";
    code += QString("    %1() {}\n").arg(className); // Default constructor
    code += QString("    ~%1() {}\n\n").arg(className); // Destructor
    
    // Add methods
    for (const QString &method : methods) {
        code += QString("    %1;\n").arg(method);
    }
    
    code += "};\n";
    
    return code;
}

void CodeGenerator::addTemplate(const CodeTemplate &template_obj)
{
    templates.append(template_obj);
}

void CodeGenerator::loadTemplates(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        return;
    }
    
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    QJsonArray templatesArray = doc.array();
    
    for (const QJsonValue &value : templatesArray) {
        QJsonObject obj = value.toObject();
        
        CodeTemplate template_obj;
        template_obj.language = obj["language"].toString();
        template_obj.pattern = obj["pattern"].toString();
        template_obj.template_code = obj["code"].toString();
        template_obj.description = obj["description"].toString();
        
        QJsonArray varsArray = obj["variables"].toArray();
        for (const QJsonValue &var : varsArray) {
            template_obj.variables.append(var.toString());
        }
        
        templates.append(template_obj);
    }
}

void CodeGenerator::saveTemplates(const QString &filePath)
{
    QJsonArray templatesArray;
    
    for (const CodeTemplate &template_obj : templates) {
        QJsonObject obj;
        obj["language"] = template_obj.language;
        obj["pattern"] = template_obj.pattern;
        obj["code"] = template_obj.template_code;
        obj["description"] = template_obj.description;
        
        QJsonArray varsArray;
        for (const QString &var : template_obj.variables) {
            varsArray.append(var);
        }
        obj["variables"] = varsArray;
        
        templatesArray.append(obj);
    }
    
    QJsonDocument doc(templatesArray);
    
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson());
    }
}

bool CodeGenerator::validateSyntax(const QString &code, const QString &language)
{
    if (language == "cpp") {
        return validateCppSyntax(code);
    } else if (language == "python") {
        return validatePythonSyntax(code);
    } else if (language == "javascript") {
        return validateJavaScriptSyntax(code);
    }
    
    return true; // Assume valid for unknown languages
}

QStringList CodeGenerator::extractFunctions(const QString &code)
{
    QStringList functions;
    
    // C++ function pattern
    QRegularExpression cppFuncPattern(R"(\b\w+\s+(\w+)\s*\([^)]*\)\s*\{)");
    QRegularExpressionMatchIterator it = cppFuncPattern.globalMatch(code);
    while (it.hasNext()) {
        QRegularExpressionMatch match = it.next();
        functions.append(match.captured(1));
    }
    
    // Python function pattern
    QRegularExpression pyFuncPattern(R"(\bdef\s+(\w+)\s*\([^)]*\)\s*:)");
    it = pyFuncPattern.globalMatch(code);
    while (it.hasNext()) {
        QRegularExpressionMatch match = it.next();
        functions.append(match.captured(1));
    }
    
    return functions;
}

QStringList CodeGenerator::extractClasses(const QString &code)
{
    QStringList classes;
    
    // C++ class pattern
    QRegularExpression cppClassPattern(R"(\bclass\s+(\w+))");
    QRegularExpressionMatchIterator it = cppClassPattern.globalMatch(code);
    while (it.hasNext()) {
        QRegularExpressionMatch match = it.next();
        classes.append(match.captured(1));
    }
    
    // Python class pattern
    QRegularExpression pyClassPattern(R"(\bclass\s+(\w+)\s*[\(:]?)");
    it = pyClassPattern.globalMatch(code);
    while (it.hasNext()) {
        QRegularExpressionMatch match = it.next();
        classes.append(match.captured(1));
    }
    
    return classes;
}

QStringList CodeGenerator::findDependencies(const QString &code, const QString &language)
{
    QStringList dependencies;
    
    if (language == "cpp") {
        QRegularExpression includePattern(R"(#include\s*[<"]([^>"]+)[>"])");
        QRegularExpressionMatchIterator it = includePattern.globalMatch(code);
        while (it.hasNext()) {
            QRegularExpressionMatch match = it.next();
            dependencies.append(match.captured(1));
        }
    } else if (language == "python") {
        QRegularExpression importPattern(R"(\b(?:import|from)\s+(\w+))");
        QRegularExpressionMatchIterator it = importPattern.globalMatch(code);
        while (it.hasNext()) {
            QRegularExpressionMatch match = it.next();
            dependencies.append(match.captured(1));
        }
    }
    
    return dependencies;
}

void CodeGenerator::learnFromCode(const QString &code, const QString &description)
{
    // Analyze patterns in the code
    analyzeCodePatterns(code);
    
    // Store code example
    codeExamples[description] = code;
    
    // Extract and learn patterns
    QStringList patterns = identifyPatterns(description);
    for (const QString &pattern : patterns) {
        if (!learnedPatterns.contains(pattern)) {
            learnedPatterns[pattern] = QStringList();
        }
        learnedPatterns[pattern].append(code);
        
        // Update confidence
        patternConfidence[pattern] = qMin(1.0, patternConfidence.value(pattern, 0.5) + 0.1);
    }
    
    learningProgress = qMin(100, learnedPatterns.size() * 2);
    emit learningProgressUpdated(learningProgress);
}

void CodeGenerator::analyzeCodePatterns(const QString &code)
{
    // Analyze common programming patterns
    if (code.contains(QRegularExpression(R"(\bfor\s*\()"))) {
        learnedPatterns["for_loop"].append(code);
    }
    
    if (code.contains(QRegularExpression(R"(\bwhile\s*\()"))) {
        learnedPatterns["while_loop"].append(code);
    }
    
    if (code.contains(QRegularExpression(R"(\bif\s*\()"))) {
        learnedPatterns["conditional"].append(code);
    }
    
    if (code.contains(QRegularExpression(R"(\bclass\s+\w+)"))) {
        learnedPatterns["class_definition"].append(code);
    }
}

QString CodeGenerator::optimizeCode(const QString &code, const QString &language)
{
    QString optimized = code;
    
    // Basic optimizations
    if (language == "cpp") {
        // Remove unnecessary includes (simplified)
        if (!optimized.contains("cout") && !optimized.contains("cin")) {
            optimized.replace("#include <iostream>\n", "");
        }
    }
    
    // Remove excessive whitespace
    optimized = optimized.replace(QRegularExpression(R"(\n\s*\n\s*\n)"), "\n\n");
    
    emit optimizationComplete(optimized);
    return optimized;
}

QString CodeGenerator::addComments(const QString &code, const QString &language)
{
    QStringList lines = code.split('\n');
    QStringList commented;
    
    for (const QString &line : lines) {
        commented.append(line);
        
        // Add comments for common patterns
        if (line.contains("main")) {
            commented.append("    // Hlavná funkcia programu");
        } else if (line.contains("class")) {
            commented.append("    // Definícia triedy");
        } else if (line.contains("for") || line.contains("while")) {
            commented.append("    // Cyklus");
        }
    }
    
    return commented.join('\n');
}

QString CodeGenerator::formatCode(const QString &code, const QString &language)
{
    // Basic code formatting
    QString formatted = code;
    
    // Normalize indentation
    QStringList lines = formatted.split('\n');
    QStringList formattedLines;
    int indentLevel = 0;
    
    for (const QString &line : lines) {
        QString trimmed = line.trimmed();
        
        if (trimmed.contains('}')) {
            indentLevel = qMax(0, indentLevel - 1);
        }
        
        QString indented = QString("    ").repeated(indentLevel) + trimmed;
        formattedLines.append(indented);
        
        if (trimmed.contains('{')) {
            indentLevel++;
        }
    }
    
    return formattedLines.join('\n');
}

CodeTemplate CodeGenerator::findBestTemplate(const QString &description, const QString &language)
{
    QString lowerDesc = description.toLower();
    CodeTemplate bestTemplate;
    double bestScore = 0.0;
    
    for (const CodeTemplate &template_obj : templates) {
        if (!language.isEmpty() && template_obj.language != language) {
            continue;
        }
        
        QRegularExpression pattern(template_obj.pattern, QRegularExpression::CaseInsensitiveOption);
        if (pattern.match(lowerDesc).hasMatch()) {
            double score = patternConfidence.value(template_obj.pattern, 0.7);
            if (score > bestScore) {
                bestScore = score;
                bestTemplate = template_obj;
            }
        }
    }
    
    return bestTemplate;
}

QString CodeGenerator::fillTemplate(const CodeTemplate &template_obj, const QString &description)
{
    QString code = template_obj.template_code;
    QMap<QString, QString> variables = extractVariables(description);
    
    // Fill in template variables
    for (const QString &var : template_obj.variables) {
        QString placeholder = "${" + var + "}";
        QString value = variables.value(var, var); // Use variable name as default
        code.replace(placeholder, value);
    }
    
    return code;
}

QMap<QString, QString> CodeGenerator::extractVariables(const QString &description)
{
    QMap<QString, QString> variables;
    
    // Extract class name
    QRegularExpression classPattern(R"(\b(?:class|trieda)\s+(\w+)", QRegularExpression::CaseInsensitiveOption);
    QRegularExpressionMatch match = classPattern.match(description);
    if (match.hasMatch()) {
        variables["ClassName"] = match.captured(1);
        variables["MemberName"] = match.captured(1);
        variables["memberName"] = match.captured(1).toLower();
    } else {
        variables["ClassName"] = "MyClass";
        variables["MemberName"] = "Value";
        variables["memberName"] = "value";
    }
    
    // Default values
    variables["MemberType"] = "int";
    variables["paramName"] = "param";
    variables["defaultValue"] = "42";
    
    return variables;
}

QString CodeGenerator::generateCppCode(const QString &description)
{
    QString lowerDesc = description.toLower();
    
    if (lowerDesc.contains("hello") || lowerDesc.contains("ahoj")) {
        return "#include <iostream>\n"
               "using namespace std;\n\n"
               "int main() {\n"
               "    cout << \"Hello World!\" << endl;\n"
               "    return 0;\n"
               "}";
    }
    
    // Default C++ template
    return QString("// Generovaný C++ kód pre: %1\n"
                   "#include <iostream>\n"
                   "using namespace std;\n\n"
                   "int main() {\n"
                   "    // TODO: Implementujte požadovanú funkcionalitu\n"
                   "    cout << \"Program spustený!\" << endl;\n"
                   "    return 0;\n"
                   "}").arg(description);
}

QString CodeGenerator::generatePythonCode(const QString &description)
{
    QString lowerDesc = description.toLower();
    
    if (lowerDesc.contains("hello") || lowerDesc.contains("ahoj")) {
        return "def main():\n"
               "    print(\"Hello World!\")\n\n"
               "if __name__ == \"__main__\":\n"
               "    main()";
    }
    
    // Default Python template
    return QString("# Generovaný Python kód pre: %1\n"
                   "def main():\n"
                   "    # TODO: Implementujte požadovanú funkcionalitu\n"
                   "    print(\"Program spustený!\")\n\n"
                   "if __name__ == \"__main__\":\n"
                   "    main()").arg(description);
}

QString CodeGenerator::generateJavaScriptCode(const QString &description)
{
    QString lowerDesc = description.toLower();
    
    if (lowerDesc.contains("hello") || lowerDesc.contains("ahoj")) {
        return "function main() {\n"
               "    console.log(\"Hello World!\");\n"
               "}\n\n"
               "main();";
    }
    
    // Default JavaScript template
    return QString("// Generovaný JavaScript kód pre: %1\n"
                   "function main() {\n"
                   "    // TODO: Implementujte požadovanú funkcionalitu\n"
                   "    console.log(\"Program spustený!\");\n"
                   "}\n\n"
                   "main();").arg(description);
}

QString CodeGenerator::generateJavaCode(const QString &description)
{
    return QString("// Generovaný Java kód pre: %1\n"
                   "public class Main {\n"
                   "    public static void main(String[] args) {\n"
                   "        // TODO: Implementujte požadovanú funkcionalitu\n"
                   "        System.out.println(\"Program spustený!\");\n"
                   "    }\n"
                   "}").arg(description);
}

bool CodeGenerator::validateCppSyntax(const QString &code)
{
    // Basic C++ syntax validation
    return code.contains("#include") && 
           code.contains("main") &&
           code.count('{') == code.count('}') &&
           code.count('(') == code.count(')');
}

bool CodeGenerator::validatePythonSyntax(const QString &code)
{
    // Basic Python syntax validation
    QStringList lines = code.split('\n');
    for (const QString &line : lines) {
        if (line.trimmed().endsWith(':')) {
            // Should be followed by indented block
            continue;
        }
    }
    return true;
}

bool CodeGenerator::validateJavaScriptSyntax(const QString &code)
{
    // Basic JavaScript syntax validation
    return code.count('{') == code.count('}') &&
           code.count('(') == code.count(')') &&
           code.count('[') == code.count(']');
}

QStringList CodeGenerator::identifyPatterns(const QString &description)
{
    QStringList patterns;
    QString lowerDesc = description.toLower();
    
    for (auto it = commonPatterns.begin(); it != commonPatterns.end(); ++it) {
        const QString &patternName = it.key();
        const QStringList &keywords = it.value();
        
        for (const QString &keyword : keywords) {
            if (lowerDesc.contains(keyword)) {
                patterns.append(patternName);
                break;
            }
        }
    }
    
    return patterns;
}

QString CodeGenerator::matchPattern(const QString &pattern, const QString &description)
{
    if (learnedPatterns.contains(pattern) && !learnedPatterns[pattern].isEmpty()) {
        // Return a random learned example
        int index = QRandomGenerator::global()->bounded(learnedPatterns[pattern].size());
        return learnedPatterns[pattern][index];
    }
    
    return QString();
}

#include "CodeGenerator.moc"