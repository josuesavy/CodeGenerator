#include "EnumDeclarator.h"

EnumDeclarator::EnumDeclarator(const std::string &output, EnumLocalizer *localizer):
    AbstractSerializer(output),
    m_localizer(localizer),
    m_fileName(ENUM_DECLARATOR_NAME)
{
}

void EnumDeclarator::serialize()
{
    m_content.clear();
    std::ostringstream out;

    out<<"#ifndef "+toUpper(QString::fromStdString(m_fileName))+"_H\n";
    out<<"#define "+toUpper(QString::fromStdString(m_fileName))+"_H\n";

    out<<"\n";

    out<<"#ifdef DELETE\n";
    out<<"#undef DELETE\n";
    out<<"#endif\n";

    out<<"\n";

    for(const EnumTranslator &child : m_localizer->getChildren())
    {
        std::string outputPath = child.getOutput();
        if (outputPath.find(m_output) == 0)
            outputPath.erase(0, m_output.length());
        if (!outputPath.empty() && outputPath[0] == '/')
            outputPath.erase(0, 1);
        out << "#include \"" << outputPath + child.getName() + ".h\"\n";
    }

    out<<"#ifdef _WIN32\n";
    out<<"#define DELETE 0\n";
    out<<"#endif\n";

    out<<"\n#endif // "+toUpper(QString::fromStdString(m_fileName))+"_H";

    m_content = out.str();
}

void EnumDeclarator::write()
{
    std::filesystem::create_directories(m_output+"/"+ENUM_UTILS_PATH);

    std::string filePath = m_output+"/"+ENUM_UTILS_PATH+"/"+m_fileName+".h";
    std::ofstream file(filePath, std::ios::out | std::ios::trunc);

    if (!file.is_open())
    {
        std::cerr << "ERREUR - EnumTranslator - Ouverture du fichier échouée: " << filePath << std::endl;
        return;
    }

    file << m_content;
    file.close();
}
