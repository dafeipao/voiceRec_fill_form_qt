#include "mainwindow.h"

#include <QApplication>
MainWindow *globalMainWindow = nullptr;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    globalMainWindow = &w;
    w.show();
    return a.exec();
}
