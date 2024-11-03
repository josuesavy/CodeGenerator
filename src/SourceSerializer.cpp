#include "SourceSerializer.h"

SourceSerializer::SourceSerializer(const std::string &output, const ClassInfos &classInfos):
    AbstractSerializer(output),
    m_header(output, classInfos),
    m_classInfos(classInfos),
    m_hasConstructor(false),
    m_needsConstructor(false)
{
    foreach(const InheritedClass &inherited, m_classInfos.inheritedClasses)
    {
        if(!inherited.passedParameters.isEmpty())
        {
            m_needsConstructor = true;
            break;
        }
    }
}

void SourceSerializer::serialize()
{
    m_header.serialize();

    m_content.clear();
    std::ostringstream out;

    out<<"#include \""<<m_classInfos.name.toStdString()+".h"<<"\"\n";

    for(const std::string &include : m_includes)
        out<<"#include \""<<include<<"\"\n";

    out<<"\n";

    if(!m_hasConstructor && m_needsConstructor)
    {
        FunctionData constructor;
        constructor.prototype.name = m_classInfos.name;
        m_functions.insert(m_functions.begin(), constructor);
    }

    for(const FunctionData &funct : m_functions)
    {
        if(!funct.prototype.returnType.type.isEmpty())
        {
            if(funct.prototype.returnType.link == SHARED_POINTER)
                out<<"QSharedPointer<";

            out<<funct.prototype.returnType.type.toStdString();

            if(funct.prototype.returnType.link == REFERENCE)
                out<<"&";

            else if(funct.prototype.returnType.link == POINTER)
                out<<"*";

            else if(funct.prototype.returnType.link == SHARED_POINTER)
                out<<">";

            out<<" ";
        }

        out<<m_classInfos.name.toStdString()<<"::"<< funct.prototype.name.toStdString()<<"(";


        for(int i = 0; i < funct.prototype.parameters.size(); i++)
        {
            if(funct.prototype.parameters[i].isConstant)
                out<<"const ";

            if(funct.prototype.parameters[i].link == SHARED_POINTER)
                out<<"QSharedPointer<";

            out<<funct.prototype.parameters[i].type.toStdString();

            if(funct.prototype.parameters[i].isContainer)
            {
                out<<"<";

                if(funct.prototype.parameters[i].containerShell.link == SHARED_POINTER)
                    out<<"QSharedPointer<";

                out<<funct.prototype.parameters[i].containerShell.type.toStdString();

                if(funct.prototype.parameters[i].containerShell.link == REFERENCE)
                    out<<"&";

                else if(funct.prototype.parameters[i].containerShell.link == POINTER)
                    out<<"*";

                else if(funct.prototype.parameters[i].containerShell.link == SHARED_POINTER)
                    out<<">";

                out<<">";
            }

            out<<" ";

            if(funct.prototype.parameters[i].link == REFERENCE)
                out<<"&";

            else if(funct.prototype.parameters[i].link == POINTER)
                out<<"*";

            else if(funct.prototype.parameters[i].link == SHARED_POINTER)
                out<<">";

            out<<funct.prototype.parameters[i].name.toStdString();

            if(i+1 != funct.prototype.parameters.size())
                out<<", ";
        }

        out<<")";

        if(funct.prototype.isConstant)
            out<<" const";

        if(funct.prototype.name == m_classInfos.name)
        {
            bool isFirst = true;

            for(const InheritedClass &inherited : m_classInfos.inheritedClasses)
            {
                if(!inherited.passedParameters.isEmpty())
                {
                    out<<"\n    ";

                    if(isFirst)
                    {
                        isFirst = false;
                        out<<":";
                    }

                    else
                        out<<",";

                    out<<inherited.name.toStdString()+"(";

                    bool isFirstP = true;

                    for(const QString &passedParam : inherited.passedParameters)
                    {
                        if(isFirstP)
                            isFirstP = false;

                        else
                            out<<", ";

                        out<<passedParam.toStdString();
                    }

                    out<<")";
                }
            }
        }

        out<<"\n";

        out<<"{\n";

        if(!funct.content.isEmpty())
            out<<"  ";

        int brackets = 1;

        for(int i = 0; i < funct.content.size(); i++)
        {
            out<<QString(funct.content[i]).toStdString();

            //Brackets
            if(i+1 != funct.content.size() && funct.content[i+1] == '}')
                brackets --;

            if(funct.content[i] == '{')
                brackets ++;

            if(i+1 != funct.content.size() && funct.content[i] == '\n')
            {
                for(int j = 0; j < brackets; j++)
                    out<<"  ";
            }
        }

        if(!funct.content.isEmpty() && funct.content.at(funct.content.size()-1) != '\n')
            out<<"\n";

        out<<"}\n";
        out<<"\n";
    }

    if(!m_hasConstructor && m_needsConstructor)
        m_functions.erase(m_functions.begin());

    m_content = out.str();
}

void SourceSerializer::write()
{
    std::filesystem::create_directories(m_output);

    std::string filePath = m_output+"/"+m_classInfos.name.toStdString()+".cpp";
    std::ofstream file(filePath, std::ios::out | std::ios::trunc);

    if (!file.is_open())
    {
        std::cerr << "ERREUR - SourceSerializer - Ouverture du fichier échouée " << m_output << std::endl;
        return;
    }

    file << m_content;
    file.close();

    m_header.write();
}

void SourceSerializer::addInclude(const std::string &include)
{
    if(std::find(m_includes.begin(), m_includes.end(), include) == m_includes.end())
        m_includes.push_back(include);
}

void SourceSerializer::addFunction(FunctionData function)
{
    m_header.addFunction(function.prototype);
    m_functions.push_back(function);
}

HeaderSerializer &SourceSerializer::getHeader()
{
    return m_header;
}
