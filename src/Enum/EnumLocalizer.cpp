#include "EnumLocalizer.h"

EnumLocalizer::EnumLocalizer(const std::string &input, const std::string &output):
    AbstractParser(input),
    AbstractSerializer(output)
{
}

void EnumLocalizer::parse()
{
    std::string path = m_input + "/" + DOFUS_ENUM_PATH;
    std::filesystem::recursive_directory_iterator dirIt(path);
    std::filesystem::recursive_directory_iterator end;

    while (dirIt != end)
    {
        if (dirIt->is_regular_file())
        {
            m_children.emplace_back(EnumTranslator(dirIt->path().string(),
                                                   m_output + "/" + ENUM_PATH + dirIt->path().parent_path().string().substr(path.size())));
            m_children.back().parse();
        }
        ++dirIt;
    }
}

void EnumLocalizer::serialize()
{
    for(int i = 0; i < m_children.size(); i++)
        m_children[i].serialize();
}

void EnumLocalizer::write()
{
    for(int i = 0; i < m_children.size(); i++)
        m_children[i].write();
}

const std::vector<EnumTranslator> &EnumLocalizer::getChildren() const
{
    return m_children;
}
