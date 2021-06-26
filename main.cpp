#include "mainwindow.h"

#include <QApplication>


int main(int argc, char *argv[])
{    
    QTextCodec *codec = QTextCodec::codecForName("ibm866");
    assert(codec);
    setlocale(LC_ALL, "russian");
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
