# Specifies project configuration and compiler options.
CONFIG += c++11 console
CONFIG -= app_bundle

# Specifies the Qt modules that are used by your project.
QT -= gui
QT += network core
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# SOFTWARE'S INFORMATIONS
# Specifies the name of the target file.
TARGET = CodeGenerator

# Defines the header files for the project.
HEADERS += \
    src/AbstractParser.h \
    src/AbstractSerializer.h \
    src/Public.h \
    src/HeaderSerializer.h \
    src/Splitter.h \
    src/SourceSerializer.h \
    src/Translator.h \
    src/Message/MessageTranslator.h \
    src/Class/ClassTranslator.h \
    src/Class/ClassLocalizer.h \
    src/Message/MessageLocalizer.h \
    src/Message/MessageDeclarator.h \
    src/GlobalTranslator.h \
    src/Class/ClassManager.h \
    src/Enum/EnumLocalizer.h \
    src/Enum/EnumTranslator.h \
    src/Enum/EnumDeclarator.h \
    src/Class/ClassDeclarator.h \
    src/Data/DataTranslator.h \
    src/Data/DataLocalizer.h \
    src/Data/DataDeclarator.h \
    src/Data/DataTypeDeclarator.h \
    src/Data/DataConverter.h \
    src/Message/MessageEnum.h \
    src/Class/ClassEnum.h \
    src/Message/MessageUtils.h

# Specifies the names of all source files in the project.
SOURCES += \
    src/AbstractParser.cpp \
    src/AbstractSerializer.cpp \
    src/main.cpp \
    src/HeaderSerializer.cpp \
    src/Splitter.cpp \
    src/SourceSerializer.cpp \
    src/Translator.cpp \
    src/Message/MessageTranslator.cpp \
    src/Class/ClassTranslator.cpp \
    src/Class/ClassLocalizer.cpp \
    src/Message/MessageLocalizer.cpp \
    src/GlobalTranslator.cpp \
    src/Message/MessageDeclarator.cpp \
    src/Class/ClassManager.cpp \
    src/Enum/EnumLocalizer.cpp \
    src/Enum/EnumTranslator.cpp \
    src/Enum/EnumDeclarator.cpp \
    src/Class/ClassDeclarator.cpp \
    src/Data/DataTranslator.cpp \
    src/Data/DataLocalizer.cpp \
    src/Data/DataDeclarator.cpp \
    src/Data/DataTypeDeclarator.cpp \
    src/Data/DataConverter.cpp \
    src/Message/MessageEnum.cpp \
    src/Class/ClassEnum.cpp \
    src/Message/MessageUtils.cpp
