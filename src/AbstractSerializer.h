#ifndef ABSTRACTSERIALIZER_H
#define ABSTRACTSERIALIZER_H

#include "Public.h"

class AbstractSerializer
{
public:
    AbstractSerializer(const std::string &output);
    virtual ~AbstractSerializer();

    virtual void serialize() = 0;
    virtual void write() = 0;

    std::string getOutput() const;

protected:
    std::string m_output;
};

#endif // ABSTRACTSERIALIZER_H
