#ifndef MESSAGEDECLARATOR_H
#define MESSAGEDECLARATOR_H

#include "src/Public.h"
#include "MessageLocalizer.h"

class MessageDeclarator : public AbstractSerializer
{
public:
    MessageDeclarator(const std::string &output, MessageLocalizer *localizer);

    virtual void serialize();
    virtual void write();

private:
    std::string m_fileName;
    std::string m_content;
    MessageLocalizer *m_localizer;
};

#endif // MESSAGEDECLARATOR_H
