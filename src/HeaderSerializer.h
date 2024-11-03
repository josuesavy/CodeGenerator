#ifndef HEADERSERIALIZER_H
#define HEADERSERIALIZER_H

#include "AbstractSerializer.h"
#include "Public.h"

#include <fstream>

class HeaderSerializer : public AbstractSerializer
{
public:
    HeaderSerializer(const std::string &output, const ClassInfos &classInfos);

    virtual void serialize();
    virtual void write();

    void addInclude(const std::string &include);
    void addFunction(FunctionPrototype prototype);
    void addClassVariable(ClassVariable variable);
    void addImplicitClass(const std::string &implicitClass);

private:
    void writeFunctionPrototype(FunctionPrototype prototype, std::ostringstream &out);
    void writeClassVariable(ClassVariable variable, std::ostringstream &out);

    bool m_hasConstructor;
    bool m_needsConstructor;
    ClassInfos m_classInfos;
    std::vector<std::string> m_includes;
    std::vector<FunctionPrototype> m_prototypes;
    std::vector<ClassVariable> m_variables;
    std::vector<std::string> m_implicitClasses;
    std::string m_content;
};

#endif // HEADERSERIALIZER_H
