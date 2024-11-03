#ifndef ENUMDECLARATOR_H
#define ENUMDECLARATOR_H

#include "src/Public.h"
#include "EnumLocalizer.h"

class EnumDeclarator : AbstractSerializer
{
public:
    EnumDeclarator(const std::string &output, EnumLocalizer *localizer);

    void serialize();
    void write();

private:
    std::string m_fileName;
    EnumLocalizer *m_localizer;
    std::string m_content;
};

#endif // ENUMDECLARATOR_H
