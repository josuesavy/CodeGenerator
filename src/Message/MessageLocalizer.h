#ifndef MESSAGELOCALIZER_H
#define MESSAGELOCALIZER_H

#include "src/Public.h"
#include "MessageTranslator.h"

class MessageLocalizer : public AbstractParser, public AbstractSerializer
{
public:
    MessageLocalizer(const std::string &input, const std::string &output);
    ~MessageLocalizer();

    virtual void parse();
    virtual void serialize();
    virtual void write();

    const QList<MessageTranslator> &getChildren() const;

private:
    void loadChild(std::string childName, std::unordered_map<std::string, int> &toLoad, std::string previous = "", bool ignorePrevious = false);

    QList<MessageTranslator> m_children;
};

#endif // MESSAGELOCALIZER_H
