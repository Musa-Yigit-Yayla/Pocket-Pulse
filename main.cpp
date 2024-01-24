#include <QWidget>
#include "registerscreen.h"
#include <QApplication>
#include "user.h"
#include "mainscreen.h"
#include "loginscreen.h"

void test0(){
    QWidget w;
    RegisterScreen rs(&w);
    w.setFixedSize(RegisterScreen::CONTAINER_FIXED_WIDTH, RegisterScreen::CONTAINER_FIXED_HEIGHT);
    w.setLayout(&rs);
    w.show();
    User* mockUser = new User("Yame dude", "password", "savecode");
    MainScreen* mc = new MainScreen(mockUser);
    LoginScreen* ls = new LoginScreen();
    mc->show();
    ls->show();
}
void defaultFlow(){
    //display login screen
    LoginScreen* ls = new LoginScreen();
    ls->show();
}
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //test0(); //remove later on
    defaultFlow();
    return a.exec();
}
