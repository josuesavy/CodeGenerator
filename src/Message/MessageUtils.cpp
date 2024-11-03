#include "MessageUtils.h"

MessageUtils::MessageUtils(const std::string &output, MessageLocalizer *localizer):
    AbstractSerializer(output),
    m_localizer(localizer)
{
}

MessageUtils::~MessageUtils()
{
    if(m_source)
        delete m_source;
}

void MessageUtils::serialize()
{
    ClassInfos classInfos;
    classInfos.name = MESSAGE_UTILS_NAME;

    m_source = new SourceSerializer(m_output+"/"+MESSAGE_UTILS_PATH, classInfos);

    m_source->getHeader().addInclude(std::string(PUBLIC_PATH));
    m_source->getHeader().addInclude(std::string(MESSAGE_UTILS_PATH)+"/"+std::string(MESSAGE_ENUM_NAME)+".h");

    FunctionData getName;
    getName.prototype.name = "getName";
    getName.prototype.isStatic = true;
    Variable parameter;
    parameter.name = "messageEnum";
    parameter.isConstant = true;
    parameter.link = LinkType::REFERENCE;
    parameter.type = "MessageEnum";
    getName.prototype.parameters<<parameter;
    getName.prototype.returnType.type = "QString";

    std::ostringstream nameOut;
    bool isFirst = true;

    for(const MessageTranslator &child : m_localizer->getChildren())
    {
        if(isFirst)
            isFirst = false;

        else
            nameOut<<"\nelse ";

        nameOut<<"if(messageEnum == "<<std::string(MESSAGE_ENUM_NAME)<<"::"<<toUpper(child.getName())<<")";
        nameOut<<"\n    return \""<<child.getName().toStdString()<<"\";";
        nameOut<<"\n";
    }

    nameOut<<"\nelse";
    nameOut<<"\n{";
    nameOut<<"\nqDebug()<<\"ERROR - "<<std::string(MESSAGE_UTILS_NAME)<<" - Don't know the enum:\"<<(int)messageEnum;";
    nameOut<<"\nreturn \"\";";
    nameOut<<"\n}";

    getName.content = QString::fromStdString(nameOut.str());
    m_source->addFunction(getName);

    m_source->serialize();
}

void MessageUtils::write()
{
    m_source->write();
}
