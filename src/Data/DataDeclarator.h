#ifndef DATADECLARATOR_H
#define DATADECLARATOR_H

#include "src/Public.h"
#include "DataLocalizer.h"

class DataDeclarator : public AbstractSerializer
{
public:
    DataDeclarator(const std::string &output, DataLocalizer *localizer);

    virtual void serialize();
    virtual void write();

private:
    std::string m_fileName;
    std::string m_content;
    DataLocalizer *m_localizer;
};

#endif // DATADECLARATOR_H
