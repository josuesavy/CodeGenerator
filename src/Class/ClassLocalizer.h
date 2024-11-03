#ifndef CLASSLOCALIZER_H
#define CLASSLOCALIZER_H

#include "src/Public.h"
#include "ClassTranslator.h"

class ClassLocalizer : public AbstractParser, public AbstractSerializer
{
public:
    ClassLocalizer(const std::string &input, const std::string &output);
    ~ClassLocalizer();

    virtual void parse();
    virtual void serialize();
    virtual void write();

    void setDerivableChildren(const QStringList &derivableClasses);
    const QList<ClassTranslator> &getChildren() const;

private:
    void orderChild(std::string childName, std::unordered_map<std::string, int> &toLoad, std::string previous = "", bool ignorePrevious = false);
    bool hasPointers(std::string childName, std::unordered_map<std::string, int> &toLoad);

    QList<ClassTranslator> m_children;
    std::vector<ClassTranslator*> m_orderedChildren;
    std::vector<std::string> m_derivableChildren;
};

#endif // CLASSLOCALIZER_H
