#ifndef DATALOCALIZER_H
#define DATALOCALIZER_H

#include "src/Public.h"
#include "DataTranslator.h"

class DataLocalizer : public AbstractParser, public AbstractSerializer
{
public:
    DataLocalizer(const std::string &input, const std::string &output);

    virtual void parse();
    virtual void serialize();
    virtual void write();

    const QList<DataTranslator> &getChildren() const;
    const QList<DataTranslator> &getUselessChildren() const;

private:
    void loadChild(std::string childName, std::unordered_map<std::string, int> &toLoad, std::string previous = "", bool ignorePrevious = false);

    QList<DataTranslator> m_children;
    QList<DataTranslator> m_uselessChildren;
};

#endif // DATALOCALIZER_H
