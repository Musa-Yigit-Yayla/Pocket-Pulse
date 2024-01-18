#include <QWidget>
#include "registerscreen.h"
#include <QApplication>
#include "user.h"
#include "mainscreen.h"


void test0(){
    User* mockUser = new User("name", "password", "savecode");
    MainScreen* mc = new MainScreen(mockUser);
    mc->show();
}
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QWidget w;
    RegisterScreen rs(&w);
    w.setFixedSize(RegisterScreen::CONTAINER_FIXED_WIDTH, RegisterScreen::CONTAINER_FIXED_HEIGHT);
    w.setLayout(&rs);
    w.show();
    //test0(); //remove later on
    return a.exec();
}
