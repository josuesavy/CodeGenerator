#ifndef CLASSENUM_H
#define CLASSENUM_H

#include "src/Public.h"
#include "ClassLocalizer.h"

class ClassEnum : public AbstractSerializer
{
public:
    ClassEnum(const std::string &output, ClassLocalizer *localizer);

    void serialize();
    void write();

private:
    std::string m_fileName;
    std::string m_content;
    ClassLocalizer *m_localizer;
};

#endif // CLASSENUM_H
