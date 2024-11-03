#ifndef DATATYPEDECLARATOR_H
#define DATATYPEDECLARATOR_H

#include "src/Public.h"
#include "src/AbstractSerializer.h"
#include "DataLocalizer.h"

class DataTypeDeclarator : public AbstractSerializer
{
public:
    DataTypeDeclarator(const std::string &output, DataLocalizer *localizer);

    virtual void serialize();
    virtual void write();

private:
    std::string m_fileName;
    std::string m_content;
    DataLocalizer *m_localizer;
};

#endif // DATATYPEDECLARATOR_H
