CONFIG += c++11 console
CONFIG -= app_bundle

QT -= gui
QT += network core
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CodeGenerator

HEADERS += \
    AbstractParser.h \
    AbstractSerializer.h \
    Public.h \
    HeaderSerializer.h \
    Splitter.h \
    SourceSerializer.h \
    Translator.h \
    Message/MessageTranslator.h \
    Class/ClassTranslator.h \
    Class/ClassLocalizer.h \
    Message/MessageLocalizer.h \
    Message/MessageDeclarator.h \
    GlobalTranslator.h \
    Class/ClassManager.h \
    Enum/EnumLocalizer.h \
    Enum/EnumTranslator.h \
    Enum/EnumDeclarator.h \
    Class/ClassDeclarator.h \
    Data/DataTranslator.h \
    Data/DataLocalizer.h \
    Data/DataDeclarator.h \
    Data/DataTypeDeclarator.h \
    Data/DataConverter.h \
    Message/MessageEnum.h \
    Class/ClassEnum.h \
    Message/MessageUtils.h

SOURCES += \
    AbstractParser.cpp \
    AbstractSerializer.cpp \
    main.cpp \
    HeaderSerializer.cpp \
    Splitter.cpp \
    SourceSerializer.cpp \
    Translator.cpp \
    Message/MessageTranslator.cpp \
    Class/ClassTranslator.cpp \
    Class/ClassLocalizer.cpp \
    Message/MessageLocalizer.cpp \
    GlobalTranslator.cpp \
    Message/MessageDeclarator.cpp \
    Class/ClassManager.cpp \
    Enum/EnumLocalizer.cpp \
    Enum/EnumTranslator.cpp \
    Enum/EnumDeclarator.cpp \
    Class/ClassDeclarator.cpp \
    Data/DataTranslator.cpp \
    Data/DataLocalizer.cpp \
    Data/DataDeclarator.cpp \
    Data/DataTypeDeclarator.cpp \
    Data/DataConverter.cpp \
    Message/MessageEnum.cpp \
    Class/ClassEnum.cpp \
    Message/MessageUtils.cpp
