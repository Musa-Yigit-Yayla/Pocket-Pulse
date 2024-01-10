#include <QWidget>
#include "mainwindow.h"
#include "registerscreen.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QWidget w;
    RegisterScreen rs(&w);
    w.setLayout(&rs);
    w.show();
    return a.exec();
}
