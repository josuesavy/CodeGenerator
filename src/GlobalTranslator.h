#ifndef GLOBATTRANSLATOR_H
#define GLOBATTRANSLATOR_H

#include "Public.h"
#include "Class/ClassDeclarator.h"
#include "Class/ClassEnum.h"
#include "Class/ClassManager.h"
#include "Message/MessageDeclarator.h"
#include "Message/MessageEnum.h"
#include "Message/MessageUtils.h"
#include "Enum/EnumDeclarator.h"
#include "Data/DataDeclarator.h"
#include "Data/DataTypeDeclarator.h"
#include "Data/DataConverter.h"

enum TranslationPart
{
    TRANSLATION_1, TRANSLATION_2
};

class GlobalTranslator : public AbstractParser, public AbstractSerializer
{
public:
    GlobalTranslator(const QString &input, const QString &output, TranslationPart translationPart);

    virtual void parse();
    virtual void serialize();
    virtual void write();

private:
    TranslationPart m_translationPart;

    // Enums..
    EnumLocalizer m_enumLocalizer;
    EnumDeclarator m_enumDeclarator;

    // Class..
    ClassLocalizer m_classLocalizer;
    ClassDeclarator m_classDeclarator;
    ClassEnum m_classEnum;
    ClassManager m_classManager;

    // Messages..
    MessageLocalizer m_messageLocalizer;
    MessageDeclarator m_messageDeclarator;
    MessageEnum m_messageEnum;
    MessageUtils m_messageUtils;

    // Datas..
    DataLocalizer m_dataLocalizer;
    DataDeclarator m_dataDeclarator;
    DataTypeDeclarator m_dataTypeDeclarator;
    DataConverter m_dataConverter;
};

#endif // GLOBATTRANSLATOR_H
