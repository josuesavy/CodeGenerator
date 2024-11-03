#include "ClassEnum.h"

ClassEnum::ClassEnum(const std::string &output, ClassLocalizer *localizer):
    AbstractSerializer(output),
    m_localizer(localizer),
    m_fileName(CLASS_ENUM_NAME)
{
}

void ClassEnum::serialize()
{
    m_content.clear();
    std::ostringstream out;

    out<<"#ifndef "+toUpper(QString::fromStdString(m_fileName))+"_H\n";
    out<<"#define "+toUpper(QString::fromStdString(m_fileName))+"_H\n";

    out<<"\n";

    out<<"\nenum class "+std::string(CLASS_ENUM_NAME);
    out<<"\n{";

    bool firstRound = true;

    for(const ClassTranslator &child : m_localizer->getChildren())
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

void ClassEnum::write()
{
    std::filesystem::create_directories(m_output+"/"+CLASS_UTILS_PATH);

    std::string filePath = m_output+"/"+CLASS_UTILS_PATH+"/"+m_fileName+".h";
    std::ofstream file(filePath, std::ios::out | std::ios::trunc);

    if (!file.is_open())
    {
        std::cerr << "ERREUR - ClassEnum - Ouverture du fichier échouée: " << filePath << std::endl;
        return;
    }

    file << m_content;
    file.close();
}
