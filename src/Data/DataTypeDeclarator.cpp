#include "DataTypeDeclarator.h"

DataTypeDeclarator::DataTypeDeclarator(const std::string &output, DataLocalizer *localizer):
    AbstractSerializer(output),
    m_localizer(localizer),
    m_fileName(std::string(DATA_TYPE_DECLARATOR_NAME))
{
}

void DataTypeDeclarator::serialize()
{
    m_content.clear();
    std::ostringstream out;

    out<<"#ifndef "+toUpper(QString::fromStdString(m_fileName))+"_H\n";
    out<<"#define "+toUpper(QString::fromStdString(m_fileName))+"_H\n";

    out<<"\n";

    out<<"\nenum class "+std::string(DATA_ENUM_TYPE_NAME);
    out<<"\n{";

    out<<"\n    UNKNOWN";

    for(const DataTranslator &child : m_localizer->getChildren())
    {
        if(!child.getModule().isEmpty())
        {
            out<<",";
            out<<"\n    "<<toUpper(child.getModule());
        }
    }


    out<<"\n};\n";

    out<<"\n#endif // "+toUpper(QString::fromStdString(m_fileName))+"_H";

    m_content = out.str();
}

void DataTypeDeclarator::write()
{
    std::filesystem::create_directories(m_output+"/"+DATA_UTILS_PATH);

    std::string filePath = m_output+"/"+DATA_UTILS_PATH+"/"+m_fileName+".h";
    std::ofstream file(filePath, std::ios::out | std::ios::trunc);

    if (!file.is_open())
    {
        std::cerr << "ERREUR - DataTypeDeclarator - Ouverture du fichier échouée: " << filePath << std::endl;
        return;
    }

    file << m_content;
    file.close();
}
