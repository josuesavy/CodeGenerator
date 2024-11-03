#include "DataDeclarator.h"

DataDeclarator::DataDeclarator(const std::string &output, DataLocalizer *localizer):
    AbstractSerializer(output),
    m_localizer(localizer),
    m_fileName(DATA_DECLARATOR_NAME)
{
}

void DataDeclarator::serialize()
{
    m_content.clear();
    std::ostringstream out;

    out<<"#ifndef "+toUpper(QString::fromStdString(m_fileName))+"_H\n";
    out<<"#define "+toUpper(QString::fromStdString(m_fileName))+"_H\n";

    out<<"\n";

    for(const DataTranslator &child : m_localizer->getChildren()){
        std::string childOutput = child.getOutput();
        if (childOutput.find(m_output) == 0)
            childOutput.erase(0, m_output.length());
        if (!childOutput.empty() && childOutput[0] == '/')
            childOutput.erase(0, 1);
        out << "#include \"" << childOutput << child.getName().toStdString() << ".h\"\n";
    }

    out<<"\nenum class "+std::string(DATA_ENUM_NAME);
    out<<"\n{";

    bool firstRound = true;

    for (const DataTranslator &child : m_localizer->getChildren())
    {
        if(firstRound)
            firstRound = false;

        else
            out<<",";

        out<<"\n    "<<toUpper(child.getName());
    }


    out<<"\n};\n";

    out<<"\n#endif // "+toUpper(QString::fromStdString(m_fileName))+"_H";

    m_content = out.str();
}

void DataDeclarator::write()
{
    std::filesystem::create_directories(m_output+"/"+DATA_UTILS_PATH);

    std::string filePath = m_output+"/"+DATA_UTILS_PATH+"/"+m_fileName+".h";
    std::ofstream file(filePath, std::ios::out | std::ios::trunc);

    if (!file.is_open())
    {
        std::cerr << "ERREUR - DataDeclarator - Ouverture du fichier échouée: " << filePath << std::endl;
        return;
    }

    file << m_content;
    file.close();
}
