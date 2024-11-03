#include "AbstractSerializer.h"

AbstractSerializer::AbstractSerializer(const std::string &output):
    m_output(output)
{
}

AbstractSerializer::~AbstractSerializer()
{
}

std::string AbstractSerializer::getOutput() const
{
    return m_output;
}
