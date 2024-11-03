#ifndef ENUMLOCALIZER_H
#define ENUMLOCALIZER_H

#include "src/Public.h"
#include "EnumTranslator.h"

class EnumLocalizer : public AbstractParser, public AbstractSerializer
{
public:
    EnumLocalizer(const std::string &input, const std::string &output);

    virtual void parse();
    virtual void serialize();
    virtual void write();

    const std::vector<EnumTranslator> &getChildren() const;

private:
    std::vector<EnumTranslator> m_children;
};

#endif // ENUMLOCALIZER_H
