#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    QTranslator translator;
//    translator.load("C:/Users/Wuql/Qt/Calendar/chinese.qm");
//    a.installTranslator(&translator);

    MainWindow w;
    w.setWindowOpacity(0.95);

    w.show();

    return a.exec();
}
