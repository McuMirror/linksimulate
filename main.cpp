#include "mainwindow.h"
#include <QApplication>
#include <QTextCodec>

#include <QThread>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication::setStyle("macintosh");
    QApplication a(argc, argv);

//    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    MainWindow w;
    w.show();

    return a.exec();
}
