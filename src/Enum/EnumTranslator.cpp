#include "EnumTranslator.h"

EnumTranslator::EnumTranslator(const std::string &input, const std::string &output):
    AbstractParser(input),
    AbstractSerializer(output),
    m_splitter(input)
{
}

void EnumTranslator::parse()
{
    m_splitter.parse();
    Translator::addConversionVariable(m_splitter.getClassInfos().name, m_splitter.getClassInfos().name, ENUM);
}

void EnumTranslator::serialize()
{
    m_content.clear();
    std::ostringstream out;

    out<<"#ifndef "<<toUpper(m_splitter.getClassInfos().name)<<"_H\n";
    out<<"#define "<<toUpper(m_splitter.getClassInfos().name)<<"_H\n";

    out<<"\nenum class "<<m_splitter.getClassInfos().name.toStdString();
    out<<"\n{";

    bool firstRound = true;

    for (const ClassVariable &classVariable : m_splitter.getClassVariables())
    {
        ConversionFunction enumVar;
        enumVar.originalName = classVariable.variable.name;
        enumVar.translatedName = QString::fromStdString(toUpper(classVariable.variable.name));

        Translator::addFunction(m_splitter.getClassInfos().name, enumVar);

        if(firstRound)
            firstRound = false;

        else
            out<<",";

        out<<"\n    "<<toUpper(classVariable.variable.name)<<" = "<<classVariable.variable.value.toStdString();
    }

    out<<"\n};\n";

    out<<"\n#endif // "<<toUpper(m_splitter.getClassInfos().name)<<"_H";

    m_content = out.str();
}

void EnumTranslator::write()
{
    std::filesystem::create_directories(m_output);

    std::string filePath = m_output + "/" + m_splitter.getClassInfos().name.toStdString() + ".h";
    std::ofstream file(filePath, std::ios::out | std::ios::trunc);

    if (!file.is_open())
    {
        std::cerr << "ERREUR - EnumTranslator - Ouverture du fichier échouée: " << filePath << std::endl;
        return;
    }

    file << m_content;
    file.close();
}

std::string EnumTranslator::getName() const
{
    return m_splitter.getClassInfos().name.toStdString();
}
