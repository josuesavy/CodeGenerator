#ifndef CLASSTRANSLATOR_H
#define CLASSTRANSLATOR_H

#include "src/Public.h"
#include "src/Translator.h"
#include "src/Splitter.h"
#include "src/SourceSerializer.h"

#include <fstream>

class ClassTranslator : public AbstractParser, public AbstractSerializer
{
public:
    ClassTranslator(const std::string &input, const std::string &output);
    ~ClassTranslator();

    virtual void parse();
    virtual void serialize();
    virtual void write();

    void addManualInclude(std::string include);
    void addAnticipatedDeclaration(std::string className, std::string include);

    QStringList getMissingIncludes() const;
    QString getName() const;
    int getId() const;
    QList<ClassVariable> getClassVariables() const;
    QList<InheritedClass> getInheritedClasses() const;

private:    
    Splitter m_splitter;
    SourceSerializer *m_source;
    int m_id;
    std::vector<std::string> m_manualIncludes;
    QStringList m_missingIncludes;
    std::vector<std::pair<std::string, std::string>> m_anticipatedDeclarations;
};

#endif // CLASSTRANSLATOR_H
