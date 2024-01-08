#include "registerscreen.h"

using namespace std;

RegisterScreen::RegisterScreen(){
    this->logoImg = new QImage(RegisterScreen::LOGO_PATH);
    QImage tempImg = (this->logoImg->scaled(LOGO_IMAGE_LENGTH, LOGO_IMAGE_LENGTH)); //use scaled method to resize the image
    this->logoImg = &tempImg;
    this->logoLabel = new QLabel();
    this->logoLabel->setPixmap(QPixmap::fromImage(*this->logoImg));


    this->nameTf = new QLineEdit();

    this->passwordTf1 = new QLineEdit();
    this->passwordTf2 = new QLineEdit();
    this->passwordTf1->setEchoMode(QLineEdit::EchoMode::Password);
    this->passwordTf2->setEchoMode(QLineEdit::EchoMode::Password);

    this->nameLabel = new QLabel(RegisterScreen::NAME_LABEL_STR);
    this->pwLabel1 = new QLabel(RegisterScreen::PASS_LABEL_STR);
    this->pwLabel2 = new QLabel(RegisterScreen::PASS_CONFIRM_LABEL_STR);
    this->setLayout();

    this->btOk = new QPushButton(QString("ok"));
    QObject::connect(this->btOk, &QPushButton::clicked, this, &RegisterScreen::btOkHandler);
}
RegisterScreen::~RegisterScreen(){

}
void RegisterScreen::setLayout(){
    this->vboxName.addWidget(this->nameTf);
    this->vboxName.addWidget(this->nameLabel);
    this->vboxPass1.addWidget(this->passwordTf1);
    this->vboxPass1.addWidget(this->pwLabel1);
    this->vboxPass2.addWidget(this->passwordTf2);
    this->vboxPass2.addWidget(this->pwLabel2);

    this->addWidget(this->logoLabel);
    this->addLayout(&this->vboxName);
    this->addLayout(&this->vboxPass1);
    this->addLayout(&this->vboxPass2);
}
void RegisterScreen::btOkHandler(){

}
const QString RegisterScreen::LOGO_PATH("C:\\Users\\yigit\\Desktop\\Qt_Container\\QT_PROJECTS\\Pocket-Pulse");
const QString RegisterScreen::NAME_LABEL_STR("Your Name: ");
const QString RegisterScreen::PASS_LABEL_STR("Password: ");
const QString RegisterScreen::PASS_CONFIRM_LABEL_STR("Confirm Password: ");
const int RegisterScreen::LOGO_IMAGE_LENGTH = 300;
