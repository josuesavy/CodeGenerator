#include "Public.h"
#include "GlobalTranslator.h"

#include <QCoreApplication>

int main(int argc, char *argv[])
{   
    QCoreApplication a(argc, argv);

    QTime timer;
    timer.start();

    GlobalTranslator globalTranslator1("C:/Users/SAVY/Documents/Dofus/Sources-Codes/" + QString(MAJ_VERSION) + "/scripts", "C:/Users/SAVY/Documents/Translate", TRANSLATION_1);

    globalTranslator1.parse();
    globalTranslator1.serialize();
    globalTranslator1.write();

    qDebug()<<"Traduction 1 terminée en"<<timer.elapsed()<<"ms";


    GlobalTranslator globalTranslator2("C:/Users/SAVY/Documents/Dofus/Sources-Codes/" + QString(MAJ_VERSION) + "/scripts", "C:/Users/SAVY/Documents/Translate", TRANSLATION_2);

    globalTranslator2.parse();
    globalTranslator2.serialize();
    globalTranslator2.write();

    qDebug()<<"Traduction 2 terminée en"<<timer.elapsed()<<"ms";

    return a.exec();
}
