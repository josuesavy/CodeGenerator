#ifndef CLASSDECLARATOR_H
#define CLASSDECLARATOR_H

#include "src/Public.h"
#include "ClassLocalizer.h"

class ClassDeclarator : public AbstractSerializer
{
public:
    ClassDeclarator(const std::string &output, ClassLocalizer *localizer);

    void serialize();
    void write();

private:
    std::string m_fileName;
    std::string m_content;
    ClassLocalizer *m_localizer;
};

#endif // CLASSDECLARATOR_H
