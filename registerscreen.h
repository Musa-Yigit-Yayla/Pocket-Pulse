#ifndef REGISTERSCREEN_H
#define REGISTERSCREEN_H

#include <QWidget>
#include <QImage>
#include <QLineEdit>
#include <QWidget>
#include <QLabel>
#include <QString>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QPushButton>

#include <string>
#include <cstddef>

using namespace std;

class RegisterScreen: public QVBoxLayout {
Q_OBJECT //use the macro to specify Qt to interpret this as a Qt related class
private:
    QImage* logoImg = nullptr;
    QLabel* logoLabel = nullptr;
    QLineEdit* nameTf = nullptr;
    QLineEdit* passwordTf1 = nullptr;
    QLineEdit* passwordTf2 = nullptr; //confirmation pass field
    QLabel* nameLabel = nullptr;
    QLabel* pwLabel1 = nullptr;
    QLabel* pwLabel2 = nullptr;
    QLabel* errorLabel = nullptr; //label for denoting any input password related warnings
    QPushButton* btOk = nullptr;

    //layout datafields
    QVBoxLayout vboxName;
    QVBoxLayout vboxPass1;
    QVBoxLayout vboxPass2;
    QVBoxLayout vboxButton; //vbox to contain btOk and error label when necessary
public:
    //some public cont fields
    static const QString LOGO_PATH;
    static const QString NAME_LABEL_STR;
    static const QString PASS_LABEL_STR;
    static const QString PASS_CONFIRM_LABEL_STR;
    static const int LOGO_IMAGE_LENGTH;

public:
    RegisterScreen();
    ~RegisterScreen();
//private methods
private:
    void setLayout();

    static const int BTOK_STRETCH = 4;

public slots:
    void btOkHandler();
};

#endif // REGISTERSCREEN_H
