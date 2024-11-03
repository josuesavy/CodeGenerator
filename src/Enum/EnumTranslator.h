#ifndef ENUMTRANSLATOR_H
#define ENUMTRANSLATOR_H

#include "src/Public.h"
#include "src/AbstractParser.h"
#include "src/AbstractSerializer.h"
#include "src/Splitter.h"
#include "src/Translator.h"

#include <fstream>

class EnumTranslator : public AbstractParser, public AbstractSerializer
{
public:
    EnumTranslator(const std::string &input, const std::string &output);

    virtual void parse();
    virtual void serialize();
    virtual void write();

    std::string getName() const;

private:
    Splitter m_splitter;
    std::string m_content;
};

#endif // ENUMTRANSLATOR_H
