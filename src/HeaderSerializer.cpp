#include "HeaderSerializer.h"

HeaderSerializer::HeaderSerializer(const std::string &output, const ClassInfos &classInfos):
    AbstractSerializer(output),
    m_classInfos(classInfos),
    m_hasConstructor(false),
    m_needsConstructor(false)
{
    for(const InheritedClass &inherited : m_classInfos.inheritedClasses)
    {
        if(!inherited.passedParameters.isEmpty())
        {
            m_needsConstructor = true;
            break;
        }
    }
}

void HeaderSerializer::serialize()
{
    m_content.clear();
    std::ostringstream out;

    out<<"#ifndef "<<toUpper(m_classInfos.name)<<"_H\n";
    out<<"#define "<<toUpper(m_classInfos.name)<<"_H\n";

    if(!m_includes.empty())
        out<<"\n";

    if(m_classInfos.isSingleton)
        out<<"#include \""<<SINGLETON_PATH<<"\"\n";

    for(const std::string &include : m_includes)
        out<<"#include \""<<include<<"\"\n";

    out<<"\n";

    if(m_classInfos.isSingleton)
        out<<"class "<<m_classInfos.name.toStdString()<<"Singleton;\n\n";


    if(!m_implicitClasses.empty())
    {
        for(const std::string &implicitClass : m_implicitClasses)
            out<<"class "<<implicitClass<<";\n";

        out<<"\n";
    }

    out<<"class "<<m_classInfos.name.toStdString();

    if(!m_classInfos.inheritedClasses.isEmpty())
    {
        out<<" : ";

        for(int i = 0; i < m_classInfos.inheritedClasses.size(); i++)
        {
            if(m_classInfos.inheritedClasses[i].inheritance == PUBLIC)
                out<<"public ";

            else if(m_classInfos.inheritedClasses[i].inheritance == PRIVATE)
                out<<"private " ;

            else
                out <<"protected ";

            out<<m_classInfos.inheritedClasses[i].name.toStdString();

            if(i+1 != m_classInfos.inheritedClasses.size())
                out<<", ";
        }
    }

    out<<"\n{";

    if(m_classInfos.isSingleton)
        out<<"\n  friend Singleton<"<<m_classInfos.name.toStdString()<<">;\n";

    bool hasAlreadyWritten = false;
    bool publicAlreadyDeclared = false;
    bool privateAlreadyDeclared = false;
    bool protectedAlreadyDeclared = false;

    if(!m_hasConstructor && m_needsConstructor)
    {
        FunctionPrototype constructor;
        constructor.name = m_classInfos.name;
        m_prototypes.insert(m_prototypes.begin(), constructor);
    }

    for(const FunctionPrototype &prototype : m_prototypes)
    {
        if(prototype.inheritance == PUBLIC)
        {
            hasAlreadyWritten = true;

            if(!publicAlreadyDeclared)
            {
                publicAlreadyDeclared = true;
                out<<"\npublic:";
            }

            writeFunctionPrototype(prototype, out);
        }
    }

    if(hasAlreadyWritten)
    {
        out<<"\n";
        hasAlreadyWritten = false;
    }

    for(const ClassVariable &variable : m_variables)
    {
        if(variable.inheritance == PUBLIC)
        {
            hasAlreadyWritten = true;

            if(!publicAlreadyDeclared)
            {
                publicAlreadyDeclared = true;
                out<<"\npublic:";
            }

            writeClassVariable(variable, out);
        }
    }

    if(hasAlreadyWritten)
    {
        out<<"\n";
        hasAlreadyWritten = false;
    }

    for(const FunctionPrototype &prototype : m_prototypes)
    {
        if(prototype.inheritance == PRIVATE)
        {
            hasAlreadyWritten = true;

            if(!privateAlreadyDeclared)
            {
                privateAlreadyDeclared = true;
                out<<"\nprivate:";
            }

            writeFunctionPrototype(prototype, out);
        }
    }

    if(hasAlreadyWritten)
    {
        out<<"\n";
        hasAlreadyWritten = false;
    }

    for(const ClassVariable &variable : m_variables)
    {
        if(variable.inheritance == PRIVATE)
        {
            hasAlreadyWritten = true;

            if(!privateAlreadyDeclared)
            {
                privateAlreadyDeclared = true;
                out<<"\nprivate:";
            }

            writeClassVariable(variable, out);
        }
    }


    if(hasAlreadyWritten)
    {
        out<<"\n";
        hasAlreadyWritten = false;
    }

    for(const FunctionPrototype &prototype : m_prototypes)
    {
        if(prototype.inheritance == PROTECTED)
        {
            hasAlreadyWritten = true;

            if(!protectedAlreadyDeclared)
            {
                protectedAlreadyDeclared = true;
                out<<"\nprotected:";
            }

            writeFunctionPrototype(prototype, out);
        }
    }

    if(hasAlreadyWritten)
    {
        out<<"\n";
        hasAlreadyWritten = false;
    }

    for(const ClassVariable &variable : m_variables)
    {
        if(variable.inheritance == PROTECTED)
        {
            hasAlreadyWritten = true;

            if(!protectedAlreadyDeclared)
            {
                protectedAlreadyDeclared = true;
                out<<"\nprotected:";
            }

            writeClassVariable(variable, out);
        }
    }

    out<<"};\n";

    if(m_classInfos.isSingleton)
    {
        out<<"\nclass "<<m_classInfos.name.toStdString()<<"Singleton : public Singleton<"<<m_classInfos.name.toStdString()<<">\n";
        out<<"{};\n";
    }

    out<<"\n#endif // "<<toUpper(m_classInfos.name)<<"_H";

    if(!m_hasConstructor && m_needsConstructor)
        m_prototypes.erase(m_prototypes.begin());

    m_content = out.str();
}

void HeaderSerializer::write()
{
    std::filesystem::create_directories(m_output);

    std::string filePath = m_output+"/"+m_classInfos.name.toStdString()+".h";
    std::ofstream file(filePath, std::ios::out | std::ios::trunc);

    if (!file.is_open())
    {
        std::cerr << "ERREUR - HeaderSerializer - Ouverture du fichier échouée " << m_output << std::endl;
        return;
    }

    file << m_content;
    file.close();
}

void HeaderSerializer::addInclude(const std::string &include)
{
    if(std::find(m_includes.begin(), m_includes.end(), include) == m_includes.end())
        m_includes.push_back(include);
}

void HeaderSerializer::addFunction(FunctionPrototype prototype)
{
    if(prototype.name == m_classInfos.name)
        m_hasConstructor = true;

    m_prototypes.push_back(prototype);
}

void HeaderSerializer::addClassVariable(ClassVariable variable)
{
    m_variables.push_back(variable);
}

void HeaderSerializer::addImplicitClass(const std::string &implicitClass)
{
    m_implicitClasses.push_back(implicitClass);
}

void HeaderSerializer::writeFunctionPrototype(FunctionPrototype prototype, std::ostringstream &out)
{
    out<<"\n  ";

    if(prototype.isVirtual)
        out<<"virtual ";

    if(prototype.isStatic)
        out<<"static ";

    if(!prototype.returnType.type.isEmpty())
    {
        if(prototype.returnType.link == SHARED_POINTER)
            out<<"QSharedPointer<";

        out<<prototype.returnType.type.toStdString();

        if(prototype.returnType.link == REFERENCE)
            out<<"&";

        else if(prototype.returnType.link == POINTER)
            out<<"*";

        else if(prototype.returnType.link == SHARED_POINTER)
            out<<">";

        out<<" ";
    }

    out<<prototype.name.toStdString()<<"(";

    for(int i = 0; i < prototype.parameters.size(); i++)
    {
        if(prototype.parameters[i].isConstant)
            out<<"const ";

        if(prototype.parameters[i].link == SHARED_POINTER)
            out<<"QSharedPointer<";

        out<<prototype.parameters[i].type.toStdString();

        if(prototype.parameters[i].isContainer)
        {
            out<<"<";

            if(prototype.parameters[i].containerShell.link == SHARED_POINTER)
                out<<"QSharedPointer<";

            out<<prototype.parameters[i].containerShell.type.toStdString();

            if(prototype.parameters[i].containerShell.link == REFERENCE)
                out<<"&";

            else if(prototype.parameters[i].containerShell.link == POINTER)
                out<<"*";

            else if(prototype.parameters[i].containerShell.link == SHARED_POINTER)
                out<<">";

            out<<">";
        }

        out<<" ";

        if(prototype.parameters[i].link == REFERENCE)
            out<<"&";

        else if(prototype.parameters[i].link == POINTER)
            out<<"*";

        else if(prototype.parameters[i].link == SHARED_POINTER)
            out<<">";

        out<<prototype.parameters[i].name.toStdString();

        if(i+1 != prototype.parameters.size())
            out<<", ";
    }

    out<<")";

    if(prototype.isConstant)
        out<<" const";

    out<<";";
}

void HeaderSerializer::writeClassVariable(ClassVariable variable, std::ostringstream &out)
{
    out<<"\n  ";

    if(variable.variable.isConstant)
        out<<"const ";

    if(variable.variable.link == SHARED_POINTER)
        out<<"QSharedPointer<";

    out<<variable.variable.type.toStdString();

    if(variable.variable.isContainer)
    {
        out<<"<";

        if(variable.variable.containerShell.link == SHARED_POINTER)
            out<<"QSharedPointer<";

        out<<variable.variable.containerShell.type.toStdString();

        if(variable.variable.containerShell.link == REFERENCE)
            out<<"&";

        else if(variable.variable.containerShell.link == POINTER)
            out<<"*";

        else if(variable.variable.containerShell.link == SHARED_POINTER)
            out<<">";

        out<<">";
    }


    if(variable.variable.link == POINTER)
        out<<"*";

    else if(variable.variable.link == REFERENCE)
        out<<"&";

    else if(variable.variable.link == SHARED_POINTER)
        out<<">";

    out<<" ";


    out<<variable.variable.name.toStdString()<<";";
}
