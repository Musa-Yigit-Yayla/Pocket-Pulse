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
#include <QMessageBox>

#include <string>
#include <cstddef>
#include <cctype>

using namespace std;

class RegisterScreen: public QVBoxLayout {
Q_OBJECT //use the macro to specify Qt to interpret this as a Qt related class
private:
    QWidget* container = nullptr;
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
    static const QString ERROR_LABEL_STYLE;
    static const int CONTAINER_FIXED_WIDTH = 250; //recommended fixed width for the container
    static const int CONTAINER_FIXED_HEIGHT = 450;

public:
    RegisterScreen(QWidget* container);
    ~RegisterScreen();

    static bool isValidPassword(const string password);
//private methods
private:
    void setLayout();
    bool retrieveConfirmation() const; //display pop-up message box

    static const int BTOK_STRETCH = 4;
    static const int NAMETF_MAXLENGTH = 120;
    static const int PASSTF_MAXLENGTH = 160;

public slots:
    void btOkHandler();
    void tf1Changed(const QString& text);
};

#endif // REGISTERSCREEN_H
