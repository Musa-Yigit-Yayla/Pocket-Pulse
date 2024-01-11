#include "registerscreen.h"
#include "user.h"
#include <iostream>

using namespace std;

//Caller is obliged to provide the correct container QWidget
RegisterScreen::RegisterScreen(QWidget* container){
    if(container != nullptr){
        this->container = container;
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
        this->passwordTf2->setVisible(false); //passwordTf2 is hidden initially

        this->nameTf->setFixedWidth(RegisterScreen::NAMETF_MAXLENGTH);
        this->passwordTf1->setFixedWidth(RegisterScreen::PASSTF_MAXLENGTH);
        this->passwordTf2->setFixedWidth(RegisterScreen::PASSTF_MAXLENGTH);

        this->nameLabel = new QLabel(RegisterScreen::NAME_LABEL_STR);
        this->pwLabel1 = new QLabel(RegisterScreen::PASS_LABEL_STR);
        this->pwLabel2 = new QLabel(RegisterScreen::PASS_CONFIRM_LABEL_STR);
        this->pwLabel2->setVisible(false);

        this->errorLabel = new QLabel();
        this->errorLabel->setStyleSheet(RegisterScreen::ERROR_LABEL_STYLE);
        this->errorLabel->setVisible(false);

        this->btOk = new QPushButton(QString("continue"));
        this->setLayout();


        QObject::connect(this->btOk, &QPushButton::clicked, this, &RegisterScreen::btOkHandler);
        QObject::connect(this->passwordTf1, &QLineEdit::textEdited, this, &RegisterScreen::tf1Changed);
    }

}
RegisterScreen::~RegisterScreen(){
    //No need to manually deallocate children widgets & layouts since Qt will handle deallocation of them automatically
}
void RegisterScreen::setLayout(){
    this->vboxName.addWidget(this->nameLabel);
    this->vboxName.addWidget(this->nameTf);
    this->vboxPass1.addWidget(this->pwLabel1);
    this->vboxPass1.addWidget(this->passwordTf1);
    this->vboxPass2.addWidget(this->pwLabel2);
    this->vboxPass2.addWidget(this->passwordTf2);


    this->addWidget(this->logoLabel);
    this->addLayout(&this->vboxName);
    this->addLayout(&this->vboxPass1);
    this->addLayout(&this->vboxPass2);

    QSpacerItem* vboxSpacer = new QSpacerItem(200, 100, QSizePolicy::Fixed, QSizePolicy::Fixed); //spacer for vbox

    this->vboxButton.addWidget(this->errorLabel, RegisterScreen::BTOK_STRETCH, Qt::AlignRight);
    this->vboxButton.addWidget(this->btOk, RegisterScreen::BTOK_STRETCH, Qt::AlignRight);

    this->addLayout(&this->vboxButton);
    this->addSpacerItem(vboxSpacer);

}
//static method to check whether a given password is acceptable
//A valid password contains uppercase and lowercase letter(s), and digit(s), and non alphanumeric character(s), and its length >= 8
bool RegisterScreen::isValidPassword(const string password){
    bool lengthSatisfied = false;

    bool containsDigit = false;
    bool containsLc = false;
    bool containsUc = false;
    bool containsNac = false; //non alpha numeric

    if(password.length() >= 8){
        lengthSatisfied = true;

        for(int i = 0; i < password.size(); i++){
            char ch = password.at(i);
            if(isdigit(ch)){
                containsDigit = true;
            }
            else if(islower(ch)){
                containsLc = true;
            }
            else if(isupper(ch)){
                containsUc = true;
            }
            else{
                containsNac = true;
            }
        }
    }

    return lengthSatisfied && containsDigit && containsLc && containsUc && containsNac;
}
//Display popup messagebox
bool RegisterScreen::retrieveConfirmation() const{
    QMessageBox confirmBox;
    confirmBox.setText(QString("Should we proceed ?"));
    confirmBox.setInformativeText(QString("Once registered, the username cannot be changed. \nAre you sure you want to proceed ?"));
    confirmBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    confirmBox.setDefaultButton(QMessageBox::Yes);

    int choice = confirmBox.exec();
    cout << "Debug: current choice in registering is " << choice << endl;
    return choice == QMessageBox::Yes;
}
void RegisterScreen::btOkHandler(){
    string pw1 = this->passwordTf1->text().toStdString();
    string pw2 = this->passwordTf2->text().toStdString();
    string givenName = this->nameTf->text().toStdString();

    if(givenName != "" && pw1 != "" && pw2 != ""){
        if(pw1 != pw2){
            this->errorLabel->setText("Passwords do not match");
            this->errorLabel->setVisible(true);
        }
        else if(RegisterScreen::isValidPassword(pw1)){
            this->errorLabel->setVisible(false);
            if(this->retrieveConfirmation()){
                //proceed with registration
                this->container->close(); //programatically close the container QWidget
                User::createUser(givenName, pw1);

                SavecodePane* svpPtr = User::getSVP();
                if(svpPtr != nullptr){
                    svpPtr->show();
                }
                else{
                    cout << "Debug: SVP pointer retrieved in RegisterScreen::btOkHandler is nullptr" << endl;
                }
            }
        }
        else{
            this->errorLabel->setText("A password must be at least length 8 and\n contain lowercase and uppercase letter(s) \n digit(s) and special character(s)");
            this->errorLabel->setVisible(true);
        }
    }
    else{
        this->errorLabel->setText("Fields cannot be empty!");
        this->errorLabel->setVisible(true);
    }

}
//Slot for handling the text change on password1
void RegisterScreen::tf1Changed(const QString& text){

    if(!this->passwordTf2->isVisible()){
        this->passwordTf2->setVisible(true);
        this->pwLabel2->setVisible(true);
    }
}
const QString RegisterScreen::LOGO_PATH("C:\\Users\\yigit\\Desktop\\Qt_Container\\QT_PROJECTS\\Pocket-Pulse\\pocket-pulse-logo.jpg");
const QString RegisterScreen::NAME_LABEL_STR("Your Name: ");
const QString RegisterScreen::PASS_LABEL_STR("Password: ");
const QString RegisterScreen::PASS_CONFIRM_LABEL_STR("Confirm Password: ");
const int RegisterScreen::LOGO_IMAGE_LENGTH = 200;
const QString RegisterScreen::ERROR_LABEL_STYLE = "QLabel {color: red;}";
