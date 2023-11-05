#ifndef ENUMTRANSLATOR_H
#define ENUMTRANSLATOR_H

#include "src/Public.h"
#include "src/AbstractParser.h"
#include "src/AbstractSerializer.h"
#include "src/Splitter.h"
#include "src/Translator.h"

class EnumTranslator : public AbstractParser, public AbstractSerializer
{
public:
    EnumTranslator(const QString &input, const QString &output);

    virtual void parse();
    virtual void serialize();
    virtual void write();

    QString getName() const;

private:
    Splitter m_splitter;
    QByteArray m_content;
};

#endif // ENUMTRANSLATOR_H
