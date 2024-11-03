#ifndef CLASSMANAGER_H
#define CLASSMANAGER_H

#include "src/Public.h"
#include "ClassLocalizer.h"

class ClassManager : public AbstractParser, public AbstractSerializer
{
public:
    ClassManager(const std::string &input, const std::string &output, ClassLocalizer *localizer);
    ~ClassManager();

    virtual void parse();
    virtual void serialize();
    virtual void write();

private:
    SourceSerializer *m_source;
    ClassLocalizer *m_localizer;
    Splitter m_splitter;
    QStringList m_derivableChildren;
};

#endif // CLASSMANAGER_H
