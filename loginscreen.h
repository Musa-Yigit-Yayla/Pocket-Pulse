#ifndef LOGINSCREEN_H
#define LOGINSCREEN_H

#include <QWidget>
#include <QImage>
#include <QTextEdit>

#include <cstddef>

using namespace std;

//Login screen
class LoginScreen: public QWidget{
private:
    QImage* logoImg = nullptr;
    QTextEdit* nameTf = nullptr;
    QTextEdit* passwordTf1 = nullptr;

public:
    LoginScreen();
};

#endif // LOGINSCREEN_H
