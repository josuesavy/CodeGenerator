#ifndef MESSAGETRANSLATOR_H
#define MESSAGETRANSLATOR_H

#include "src/Public.h"
#include "src/Translator.h"
#include "src/Splitter.h"
#include "src/SourceSerializer.h"

#include <fstream>

class MessageTranslator : public AbstractParser, public AbstractSerializer
{
public:
    MessageTranslator(const std::string &input, const std::string &output);
    ~MessageTranslator();

    virtual void parse();
    virtual void serialize();
    virtual void write();

    void addManualInclude(std::string include);
    void addAnticipatedDeclaration(std::string className, std::string include);

    QStringList getMissingIncludes() const;
    QString getName() const;
    int getId() const;

private:
    Splitter m_splitter;
    SourceSerializer *m_source;
    int m_id;
    std::vector<std::string> m_manualIncludes;
    QStringList m_missingIncludes;
    std::vector<std::pair<std::string, std::string>> m_anticipatedDeclarations;
};

#endif // MESSAGETRANSLATOR_H
