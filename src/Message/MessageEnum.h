#ifndef MESSAGEENUM_H
#define MESSAGEENUM_H

#include "src/Public.h"
#include "MessageLocalizer.h"

class MessageEnum : public AbstractSerializer
{
public:
    MessageEnum(const std::string &output, MessageLocalizer *localizer);

    virtual void serialize();
    virtual void write();

private:
    std::string m_fileName;
    std::string m_content;
    MessageLocalizer *m_localizer;
};

#endif // MESSAGEENUM_H
