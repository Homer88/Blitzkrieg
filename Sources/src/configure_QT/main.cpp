#include "mainwindow.h"

#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
      // Загружаем файл из ресурсов (путь начинается с ":/")
     //QString transPath = QCoreApplication::applicationDirPath() + "/translations/
       if (translator.load("translate/configure/Configure_ru_RU.qm")) {
           a.installTranslator(&translator);
           qDebug() << "Translation loaded successfully from resources!";
       } else {
           qDebug() << "Failed to load translation from resources.";
       }

    MainWindow w;
    w.show();
    return a.exec();
}
