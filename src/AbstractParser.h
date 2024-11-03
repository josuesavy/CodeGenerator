#ifndef ABSTRACTPARSER_H
#define ABSTRACTPARSER_H

#include "AbstractSerializer.h"

class AbstractParser
{
public:
    AbstractParser(const std::string &input);
    virtual ~AbstractParser();

    virtual void parse() = 0;

protected:
    std::string m_input;
};

#endif // ABSTRACTPARSER_H
