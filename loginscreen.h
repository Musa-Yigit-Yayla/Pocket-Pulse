#ifndef LOGINSCREEN_H
#define LOGINSCREEN_H

#include "maincontroller.h"
#include <QWidget>
#include <QImage>
#include <QTextEdit>
#include <QPushButton>
#include <cstddef>
#include <QGridLayout>

using namespace std;

//Login screen
class LoginScreen: public QWidget{
private:
    QImage* logoImg = nullptr;
    QTextEdit* nameTf = nullptr;
    QTextEdit* passwordTf1 = nullptr;
    QPushButton* btRegdirect = nullptr;
    QPushButton* btLogin = nullptr;
    QPushButton* btHelpdirect = nullptr;
    QPushButton* btLoginDirect = nullptr; //redirect back to login screen from help screen
    QGridLayout* gridPane = nullptr;

public:
    LoginScreen();
};

#endif // LOGINSCREEN_H
