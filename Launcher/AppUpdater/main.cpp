#include <QApplication>
#include <QTranslator>
#include <QLocale>

#include "Updater.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);    

    QTranslator translator;
    QString language = "en";
    switch(QLocale::system().language())
    {
    case QLocale::French: language = "fr"; break;
    default: language = "en";
    }

    QString transFile = QString(QString(":/i18n/%1").arg(language));
    translator.load(transFile);
    a.installTranslator(&translator);

    Updater updater;
    return updater.start(argc, argv);
}
