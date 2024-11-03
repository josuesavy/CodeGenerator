#ifndef SPLITTER_H
#define SPLITTER_H

#include "Public.h"
#include "AbstractParser.h"

#include <QRegularExpression>

class Splitter : public AbstractParser
{
public:
    Splitter(const std::string &input);

    virtual void parse();

    QList<QString> getIncludes() const;
    ClassInfos getClassInfos() const;
    QList<FunctionData> getFunctions() const;
    QList<ClassVariable> getClassVariables() const;

    //
//    static const QString includePattern = "^import\s+(?:(?:([\w.]+)\s+as\s+([\w]+)|([\w.]+)|\*)|([\w.]+)\.([\w]+)|([\w.]+)(?:,\s+([\w.]+))*)\s*;$";
//    static const QString classPattern = "/(?:(public|private|protected|internal|final|dynamic|static|override|public static|private static|protected static|internal static)?\s+)?(?:class|interface)\s+(\w+)(?:\s+extends\s+(\w+))?(?:\s+implements\s+([\w\s,]+))?\s*\{/gm";
//    static const QString functionPattern = "^\s*(override)?\s*(public|private|protected|internal)?\s*function\s+(get|set)?\s*(\w+)\s*\(([^)]*)\)\s*:\s*([\w.]+)\s*{";
//    static const QString paramFunctionPattern = "(\w+):\s*([^\s,]+)(?:\s*=\s*([^,\)]+))?";
//    static const QString fieldPattern = "^\s*(public|private|protected)?\s+(static\s+)?var\s+(?<name>[a-zA-Z_]\w*)\s*:\s*(?<type>[^;<>]+(?:<(?:[^<>]+|(?&type))*>)?)\s*;\s*$";

private:
    void readInclude(QString line);
    void readClass(QString line);
    void readPrototype(QString line);
    void readContent(QString line, int brackets);
    void readClassVariable(QString line);

    QList<FunctionData> m_functions;
    QList<QString> m_includes;
    ClassInfos m_classInfos;
    QList<ClassVariable> m_variables;
};

#endif // SPLITTER_H
