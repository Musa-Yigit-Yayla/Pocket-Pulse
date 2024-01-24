#include "loginscreen.h"

LoginScreen::LoginScreen(): QWidget{nullptr}{
    //establish database connection
    this->mc = new MainController();

    this->logoImg = new QImage(RegisterScreen::LOGO_PATH);
    this->nameTf = new QLineEdit();
    this->passwordTf = new QLineEdit();
    this->passwordTf->setEchoMode(QLineEdit::Password);
    this->btHelpdirect = new QPushButton("Need help ?");
    this->btHelpdirect->setFlat(true);
    this->btLogin = new QPushButton("login");
    this->btRegdirect = new QPushButton("Register ?");
    this->btRegdirect->setFlat(true);
    this->nameLabel = new QLabel("Name:");
    this->pwLabel = new QLabel("Password:");

    this->setLayoutManagement();
    QObject::connect(this->btRegdirect, &QPushButton::clicked, this, &LoginScreen::slotRegdirect);
    QObject::connect(this->btLogin, &QPushButton::clicked, this, &LoginScreen::slotLogin);
    QObject::connect(this->btLoginDirect, &QPushButton::clicked, this, &LoginScreen::slotLoginDirect);
    QObject::connect(this->btHelpdirect, &QPushButton::clicked, this, &LoginScreen::slotHelpdirect);
}
void LoginScreen::setLayoutManagement(){
    this->gridPane = new QGridLayout(this);
    //this->gridPane->setHorizontalSpacing(20);
    //this->gridPane->setVerticalSpacing(20);
    this->gridPane->addWidget(this->nameLabel, 0, 0);
    this->gridPane->addWidget(this->nameTf, 1, 0, 1, 2);
    this->gridPane->addWidget(this->pwLabel, 2, 0);
    this->gridPane->addWidget(this->passwordTf, 3, 0, 1, 2);
    this->gridPane->addWidget(this->btLogin, 4, 2);
    this->gridPane->addWidget(this->btHelpdirect, 7, 0);
    this->gridPane->addWidget(this->btRegdirect, 8, 0);
}
void LoginScreen::slotRegdirect(){

}
void LoginScreen::slotLogin(){

}
void LoginScreen::slotLoginDirect(){

}
void LoginScreen::slotHelpdirect(){

}
void LoginScreen::paintEvent(QPaintEvent* event){
    QPainter painter; //SET THE PAINTER'S DESIGNATED TARGET
    const QRect& boundingRect = QRect(0, 0, this->width(), this->height());
    painter.drawImage(boundingRect, *this->logoImg);
}
