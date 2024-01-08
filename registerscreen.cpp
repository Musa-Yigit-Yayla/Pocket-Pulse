#include "registerscreen.h"

using namespace std;

RegisterScreen::RegisterScreen(){
    this->logoImg = new QImage(RegisterScreen::LOGO_PATH);
    this->nameTf = new QLineEdit();

    this->passwordTf1 = new QLineEdit();
    this->passwordTf2 = new QLineEdit();
    this->passwordTf1->setEchoMode(QLineEdit::EchoMode::Password);
    this->passwordTf2->setEchoMode(QLineEdit::EchoMode::Password);

    this->nameLabel = new QLabel(RegisterScreen::NAME_LABEL_STR);
    this->pwLabel1 = new QLabel(RegisterScreen::PASS_LABEL_STR);
    this->pwLabel2 = new QLabel(RegisterScreen::PASS_CONFIRM_LABEL_STR);
}
RegisterScreen::~RegisterScreen(){

}


const QString RegisterScreen::LOGO_PATH("C:\\Users\\yigit\\Desktop\\Qt_Container\\QT_PROJECTS\\Pocket-Pulse");
const QString RegisterScreen::NAME_LABEL_STR("Your Name: ");
const QString RegisterScreen::PASS_LABEL_STR("Password: ");
const QString RegisterScreen::PASS_CONFIRM_LABEL_STR("Confirm Password: ");
