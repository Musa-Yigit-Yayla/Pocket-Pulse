#ifndef REGISTERSCREEN_H
#define REGISTERSCREEN_H

#include <QWidget>
#include <QImage>
#include <QLineEdit>
#include <QLabel>
#include <QString>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QPushButton>

#include <string>
#include <cstddef>

using namespace std;

class RegisterScreen: public QVBoxLayout {
public:
    RegisterScreen();
    ~RegisterScreen();

    //some public cont fields
    static const QString LOGO_PATH;
    static const QString NAME_LABEL_STR;
    static const QString PASS_LABEL_STR;
    static const QString PASS_CONFIRM_LABEL_STR;
private:
    QImage* logoImg = nullptr;
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
};

#endif // REGISTERSCREEN_H
