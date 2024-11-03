#ifndef DATATRANSLATOR_H
#define DATATRANSLATOR_H

#include "src/Public.h"
#include "src/AbstractParser.h"
#include "src/AbstractSerializer.h"
#include "src/Splitter.h"
#include "src/Translator.h"
#include "src/SourceSerializer.h"

#include <fstream>

class DataTranslator : public AbstractParser, public AbstractSerializer
{
public:
    DataTranslator(const std::string &input, const std::string &output);
    ~DataTranslator();

    virtual void parse();
    virtual void serialize();
    virtual void write();

    void addManualInclude(std::string include);
    void addAnticipatedDeclaration(std::string className, std::string include);

    QStringList getMissingIncludes() const;
    QString getName() const;
    QString getDofusName() const;
    QStringList getRequiredIncludes() const;
    QString getModule() const;

private:
    QString getFieldTranslatedLine(Variable variable, bool isFirst);

    Splitter m_splitter;
    SourceSerializer *m_source;
    std::vector<std::string> m_manualIncludes;
    QStringList m_missingIncludes;
    std::vector<std::pair<std::string, std::string>> m_anticipatedDeclarations;
    QString m_module;
};

#endif // DATATRANSLATOR_H
