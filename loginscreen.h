#ifndef LOGINSCREEN_H
#define LOGINSCREEN_H

#include "maincontroller.h"
#include "mainscreen.h"
#include "registerscreen.h"
#include <QWidget>
#include <QImage>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <cstddef>
#include <QGridLayout>
#include <QPainter>
#include <QPaintEvent>
#include <QComboBox>
#include <qDebug>
#include <string>

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
    QLabel* errorLabel = nullptr;

    QGridLayout* helpPane1 = nullptr; //stage 1
    QComboBox* namesBox = nullptr;
    QLineEdit* savecodeTf = nullptr;
    QLabel* savecodeLabel = nullptr;
    QPushButton* btResetPass = nullptr;
    QGridLayout* helpPane2 = nullptr; //stage 1


public:
    LoginScreen();
    void paintEvent(QPaintEvent*) override;

    static const int FIXED_WIDTH = 250;
    static const int FIXED_HEIGHT = 350;
private:
    void setLayoutManagement();
    static const string EMPTY_FIELD_STR;
    static const string USER_DNE_STR;
    static const string PASSWORD_MISMATCH_STR;

public slots:
    void slotRegdirect();
    void slotLogin();
    void slotLoginDirect();
    void slotHelpdirect();
};

#endif // LOGINSCREEN_H
