#include "MessageEnum.h"

MessageEnum::MessageEnum(const std::string &output, MessageLocalizer *localizer):
    AbstractSerializer(output),
    m_localizer(localizer),
    m_fileName(MESSAGE_ENUM_NAME)
{
}

void MessageEnum::serialize()
{
    m_content.clear();
    std::ostringstream out;

    out<<"#ifndef "+toUpper(QString::fromStdString(m_fileName))+"_H\n";
    out<<"#define "+toUpper(QString::fromStdString(m_fileName))+"_H\n";

    out<<"\n";

    out<<"\nenum class "+std::string(MESSAGE_ENUM_NAME);
    out<<"\n{";

    bool firstRound = true;

    for(const MessageTranslator &child : m_localizer->getChildren())
    {
        if(firstRound)
            firstRound = false;

        else
            out<<",";

        out<<"\n    "<<toUpper(child.getName())<<" = "<<child.getId();
    }


    out<<"\n};\n";

    out<<"\n#endif // "+toUpper(QString::fromStdString(m_fileName))+"_H";

    m_content = out.str();
}

void MessageEnum::write()
{
    std::filesystem::create_directories(m_output+"/"+MESSAGE_UTILS_PATH);

    std::string filePath = m_output+"/"+MESSAGE_UTILS_PATH+"/"+m_fileName+".h";
    std::ofstream file(filePath, std::ios::out | std::ios::trunc);

    if (!file.is_open())
    {
        std::cerr << "ERREUR - MessageEnum - Ouverture du fichier échouée: " << filePath << std::endl;
        return;
    }

    file << m_content;
    file.close();
}
