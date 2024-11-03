#include "MessageTranslator.h"

MessageTranslator::MessageTranslator(const std::string &input, const std::string &output):
    AbstractParser(input),
    AbstractSerializer(output),
    m_splitter(input),
    m_source(NULL),
    m_id(-1)
{
}

MessageTranslator::~MessageTranslator()
{
    if(m_source)
        delete m_source;
}

void MessageTranslator::parse()
{
    m_splitter.parse();

    foreach(const FunctionData &funct, m_splitter.getFunctions())
    {
        if(funct.prototype.name == "getMessageId")
        {
            m_id = funct.content.split(" ", Qt::SkipEmptyParts)[1].split(";", Qt::SkipEmptyParts)[0].toInt();
            break;
        }
    }

    ClassInfos classInfos = m_splitter.getClassInfos();

    for(int i = 0; i < classInfos.inheritedClasses.size(); i++)
    {
        if(Translator::isTranslated(classInfos.inheritedClasses[i].name) &&
                !m_missingIncludes.contains(classInfos.inheritedClasses[i].name))
            m_missingIncludes<<classInfos.inheritedClasses[i].name;
    }
}

void MessageTranslator::serialize()
{
    ClassInfos classInfos = m_splitter.getClassInfos();
    QList<ClassVariable> classVariables = m_splitter.getClassVariables();

    for(int i = 0; i < classInfos.inheritedClasses.size(); i++)
    {
        if(!Translator::isTranslated(classInfos.inheritedClasses[i].name))
        {
//            if(!Translator::isKnown(classInfos.inheritedClasses[i].name))
//                qWarning()<<"ERREUR - MessageTranslator - Ne connait pas"<<classInfos.inheritedClasses[i].name<<"herite par la classe"<<getName();

            /*else*/ if(classInfos.inheritedClasses[i].name == "INetworkDataContainerMessage")
            {
                ClassVariable container;
                container.variable.name = "content";
                container.variable.type = "ByteArray";
                classVariables<<container;
            }

            classInfos.inheritedClasses.removeAt(i);
            i--;
        }
    }

    if(classInfos.inheritedClasses.isEmpty())
    {
        InheritedClass baseClass;
        baseClass.name = MESSAGE_BASE_NAME;
        classInfos.inheritedClasses<<baseClass;
    }

    m_source = new SourceSerializer(m_output, classInfos);

    foreach(const QString &include, m_splitter.getIncludes())
    {
        if(!include.contains("message"))
        {
            std::string translatedInclude = Translator::translateInclude(include).toStdString();

            if(!translatedInclude.empty())
                m_source->getHeader().addInclude(translatedInclude);
        }
    }

    m_source->getHeader().addInclude(std::string(MESSAGE_UTILS_PATH)+"/"+std::string(MESSAGE_BASE_NAME)+".h");

    foreach(const std::string &include, m_manualIncludes)
        m_source->getHeader().addInclude(include);

    for(int i = 0; i < m_anticipatedDeclarations.size(); i++)
    {
        m_source->getHeader().addImplicitClass(m_anticipatedDeclarations[i].first);
        m_source->addInclude(m_anticipatedDeclarations[i].second);
    }

    foreach(const QString &include, m_splitter.getIncludes())
    {
        std::string translated = Translator::translateInclude(include).toStdString();

        if(!translated.empty())
            m_source->getHeader().addInclude(translated);
    }

    QStringList content;

    foreach(const FunctionData &funct, m_splitter.getFunctions())
    {
        if(funct.prototype.name.contains("serialize"))
        {
            ConversionFunction conversionFunct;
            conversionFunct. isSelfActionNotDetainedByClass = false;
            conversionFunct.originalName = funct.prototype.name;
            conversionFunct.translatedName = funct.prototype.name;
            conversionFunct.parameters<<"#0#";
            Translator::addFunction(m_splitter.getClassInfos().name, conversionFunct);
        }

        else if(funct.prototype.name == "getTypeId")
        {
            ConversionFunction conversionFunct;
            conversionFunct. isSelfActionNotDetainedByClass = false;
            conversionFunct.originalName = funct.prototype.name;
            conversionFunct.translatedName = "getType";
            Translator::addFunction(m_splitter.getClassInfos().name, conversionFunct);
        }
    }

    bool needsHash = false;

    foreach(const FunctionData &funct, m_splitter.getFunctions())
    {
        if(funct.prototype.name.contains("serialize"))
        {
            FunctionData translated = Translator::translateFunction(funct, classVariables, classInfos);

            if(funct.prototype.name == "serialize" || funct.prototype.name == "deserialize")
                translated.prototype.isVirtual = true;

            m_source->addFunction(translated);
            content<<translated.content;
        }

        if(funct.prototype.name.contains("_") && funct.prototype.name.contains("Func"))
        {
            FunctionData translated = Translator::translateFunction(funct, classVariables, classInfos);

            m_source->addFunction(translated);
            content<<translated.content;
        }

        if(funct.prototype.name == "pack" && funct.content.contains("HASH_FUNCTION"))
            needsHash = true;
    }

    FunctionData constructor;
    constructor.prototype.name = m_splitter.getClassInfos().name;

    QTextStream outConstructor(&constructor.content, QIODevice::WriteOnly);

    outConstructor<<"m_type = "<<MESSAGE_ENUM_NAME<<"::"<<getName().toUpper()<<";";

    if(needsHash)
        outConstructor<<"\nm_needsHash = true;";

    foreach(const ClassVariable &variable, classVariables)
    {
        if(!content.filter(variable.variable.name).isEmpty())
        {
            ClassVariable translatedVariable = Translator::translateClassVariable(variable);

//            // ADD
//            if(translatedVariable.variable.name == "register")
//                translatedVariable.variable.name = "m_"+translatedVariable.variable.name;

            m_source->getHeader().addClassVariable(translatedVariable);
        }
    }

    if(!constructor.content.isEmpty())
        m_source->addFunction(constructor);

    m_source->serialize();
}

void MessageTranslator::write()
{
    m_source->write();
}

void MessageTranslator::addManualInclude(std::string include)
{
    m_manualIncludes.push_back(include);
}

void MessageTranslator::addAnticipatedDeclaration(std::string className, std::string include)
{
    std::pair<std::string, std::string> pair;
    pair.first = className;
    pair.second = include;
    m_anticipatedDeclarations.push_back(pair);
}

QStringList MessageTranslator::getMissingIncludes() const
{
    return m_missingIncludes;
}

QString MessageTranslator::getName() const
{
    return m_splitter.getClassInfos().name;
}

int MessageTranslator::getId() const
{
    return m_id;
}
