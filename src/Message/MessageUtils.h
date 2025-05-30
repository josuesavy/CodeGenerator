#ifndef MESSAGEUTILS_H
#define MESSAGEUTILS_H

#include "src/Public.h"
#include "MessageLocalizer.h"

class MessageUtils : public AbstractSerializer
{
public:
    MessageUtils(const std::string &output, MessageLocalizer *localizer);
    ~MessageUtils();

    virtual void serialize();
    virtual void write();

private:
    SourceSerializer *m_source;
    MessageLocalizer *m_localizer;
};

#endif // MESSAGEUTILS_H
