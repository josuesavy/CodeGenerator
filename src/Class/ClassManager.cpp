#include "ClassManager.h"

ClassManager::ClassManager(const std::string &input, const std::string &output, ClassLocalizer *localizer):
    AbstractParser(input),
    AbstractSerializer(output),
    m_localizer(localizer),
    m_splitter(input+"/"+DOFUS_PROTOCOLTYPEMANAGER_PATH)
{
}

ClassManager::~ClassManager()
{
    if(m_source)
        delete m_source;
}

void ClassManager::parse()
{
    m_splitter.parse();

    std::string content;
    for(const FunctionData &funct : m_splitter.getFunctions())
    {
        if(funct.prototype.name == "register")
        {
            content = funct.content.toStdString();
            break;
        }
    }


    std::istringstream in(content);
    std::string line;

    do
    {
        std::getline(in, line);
        line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end());

        if (line.find("StoreDataManager.getInstance().registerClass(new ") != std::string::npos)
        {
            line.erase(0, line.find("new ") + 4);
            line.erase(line.find("(),true,true);"));

            if (std::find(m_derivableChildren.begin(), m_derivableChildren.end(), line) == m_derivableChildren.end())
                m_derivableChildren<<QString::fromStdString(line);
        }

    } while (!in.eof());

    m_localizer->setDerivableChildren(m_derivableChildren);
}

void ClassManager::serialize()
{
    ClassInfos classInfos;
    classInfos.name = CLASS_MANAGER_NAME;
    classInfos.isSingleton = true;

    m_source = new SourceSerializer(m_output+"/"+CLASS_UTILS_PATH, classInfos);

    m_source->getHeader().addInclude(std::string(CLASS_UTILS_PATH)+"/"+std::string(CLASS_BASE_NAME)+".h");

    for(const QString &include : m_splitter.getIncludes())
    {
        std::string translated = Translator::translateInclude(include).toStdString();
        if(!translated.empty())
            m_source->addInclude(translated);
    }

    FunctionData getClass;
    getClass.prototype.name = "getClass";
    getClass.prototype.returnType.type = CLASS_BASE_NAME;
    getClass.prototype.returnType.link = SHARED_POINTER;
    Variable parameter;
    parameter.type = "int";
    parameter.name = "classId";
    getClass.prototype.parameters<<parameter;

    std::ostringstream out;
    out<<"switch(classId)\n";
    out<<"{\n";

    std::unordered_map<std::string, int> table;

    for(const ClassTranslator &child : m_localizer->getChildren())
        table[child.getName().toStdString()] = child.getId();

    out<<"default:\n";
    out<<"{\n";
    out<<"qDebug()<<\"ERREUR - ClassManager - Ne connait de classe possedant l'id\"<<"<<parameter.name.toStdString()<<";\n";
    out<<"return QSharedPointer<"+std::string(CLASS_BASE_NAME)+">();\n";
    out<<"}\n\n";

    for(const QString &derivableChild : m_derivableChildren)
    {
        if (table.find(derivableChild.toStdString()) != table.end())
        {
            out<<"case "<<table[derivableChild.toStdString()]<<":\n";
            out<<"  return QSharedPointer<"+std::string(CLASS_BASE_NAME)+">(new "<<derivableChild.toStdString()<<"());\n";
            out<<"\n";
        }

        else
            std::cerr<<"ERREUR - ClassManager - Ne connait pas la classe"<<derivableChild.toStdString()<<std::endl;
    }

    out<<"}\n";

    getClass.content = QString::fromStdString(out.str());
    m_source->addFunction(getClass);

    m_source->serialize();
}

void ClassManager::write()
{
    m_source->write();
}
