#ifndef SOURCESERIALIZER_H
#define SOURCESERIALIZER_H

#include "AbstractSerializer.h"
#include "HeaderSerializer.h"

#include <fstream>

class SourceSerializer : public AbstractSerializer
{
public:
    SourceSerializer(const std::string &output, const ClassInfos &classInfos);

    virtual void serialize();
    virtual void write();

    void addInclude(const std::string &include);
    void addFunction(FunctionData function);

    HeaderSerializer &getHeader();

private:
    bool m_hasConstructor;
    bool m_needsConstructor;
    HeaderSerializer m_header;
    ClassInfos m_classInfos;
    std::vector<FunctionData> m_functions;
    std::vector<std::string> m_includes;
    std::string m_content;
};

#endif // SOURCESERIALIZER_H
