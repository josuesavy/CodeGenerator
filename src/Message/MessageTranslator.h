#ifndef MESSAGETRANSLATOR_H
#define MESSAGETRANSLATOR_H

#include "src/Public.h"
#include "src/Translator.h"
#include "src/Splitter.h"
#include "src/SourceSerializer.h"

class MessageTranslator : public AbstractParser, public AbstractSerializer
{
public:
    MessageTranslator(const QString &input, const QString &output);
    ~MessageTranslator();

    virtual void parse();
    virtual void serialize();
    virtual void write();

    void addManualInclude(QString include);
    void addAnticipatedDeclaration(QString className, QString include);

    QStringList getMissingIncludes() const;
    QString getName() const;
    int getId() const;

private:
    Splitter m_splitter;
    SourceSerializer *m_source;
    int m_id;
    QStringList m_manualIncludes;
    QStringList m_missingIncludes;
    QList<QPair<QString, QString>> m_anticipatedDeclarations;
};

#endif // MESSAGETRANSLATOR_H
