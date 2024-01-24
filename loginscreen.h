#ifndef LOGINSCREEN_H
#define LOGINSCREEN_H

#include "maincontroller.h"
#include <QWidget>
#include <QImage>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <cstddef>
#include <QGridLayout>
#include <QPainter>
#include <QPaintEvent>

using namespace std;

//Login screen
class LoginScreen: public QWidget{
private:
    MainController* mc = nullptr;

    QImage* logoImg = nullptr;
    QLineEdit* nameTf = nullptr;
    QLineEdit* passwordTf = nullptr;
    QPushButton* btRegdirect = nullptr;
    QPushButton* btLogin = nullptr;
    QPushButton* btHelpdirect = nullptr;
    QPushButton* btLoginDirect = nullptr; //redirect back to login screen from help screen
    QGridLayout* gridPane = nullptr;
    QLabel* nameLabel = nullptr;
    QLabel* pwLabel = nullptr;


public:
    LoginScreen();
    void paintEvent(QPaintEvent*) override;
private:
    void setLayoutManagement();

public slots:
    void slotRegdirect();
    void slotLogin();
    void slotLoginDirect();
    void slotHelpdirect();
};

#endif // LOGINSCREEN_H
