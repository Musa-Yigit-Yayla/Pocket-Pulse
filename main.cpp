#include <QWidget>
#include "registerscreen.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QWidget w;
    RegisterScreen rs(&w);
    w.setFixedSize(RegisterScreen::CONTAINER_FIXED_WIDTH, RegisterScreen::CONTAINER_FIXED_HEIGHT);
    w.setLayout(&rs);
    w.show();
    return a.exec();
}
