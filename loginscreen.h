#ifndef LOGINSCREEN_H
#define LOGINSCREEN_H

class MainController;

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
#include <QPixmap>
#include <QVBoxLayout>
#include <QRect>
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
    QWidget* helpContainer1 = nullptr;
    QLabel* usernameLabel = nullptr;
    QComboBox* namesBox = nullptr;
    QLineEdit* savecodeTf = nullptr;
    QLabel* savecodeLabel = nullptr;
    QPushButton* btResetPass = nullptr;
    QLabel* errorLabel1 = nullptr;

    QGridLayout* helpPane2 = nullptr; //stage 2
    QWidget* helpContainer2 = nullptr;
    QLabel* passLabel1 = nullptr;
    QLabel* passLabel2 = nullptr;
    QLineEdit* passTf1 = nullptr;
    QLineEdit* passTf2 = nullptr;
    QLabel* errorLabel2 = nullptr;
    QLabel* expLabel2 = nullptr;
    QPushButton* btResetAccept = nullptr;
    QPushButton* btReturn = nullptr;

public:
    LoginScreen();
    void paintEvent(QPaintEvent*) override;

    static const int FIXED_WIDTH = 250;
    static const int FIXED_HEIGHT = 350;
private:
    void setLayoutManagement();
    void setLayoutHelp1();

    static const string EMPTY_FIELD_STR;
    static const string USER_DNE_STR;
    static const string PASSWORD_MISMATCH_STR;
    static const string SAVECODE_MISMATCH_STR;
    static const int BT_BACK_LENGTH = 15;

public slots:
    void slotRegdirect();
    void slotLogin();
    void slotLoginDirect();
    void slotHelpdirect();
    void slotResetVerify();
    void slotResetAccept();
    void slotReturnDirect();
};

#endif // LOGINSCREEN_H
