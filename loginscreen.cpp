#include "loginscreen.h"
#include "maincontroller.h"
#include "mainscreen.h"
#include "registerscreen.h"
#include "user.h"
#include <iostream>

using namespace std;

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
    this->errorLabel = new QLabel();
    this->errorLabel->setStyleSheet("color: red;");
    this->errorLabel->setVisible(false);

    this->btHelpdirect->setStyleSheet("text-decoration: underline; color: rgb(0, 0, 255);");
    this->btRegdirect->setStyleSheet("text-decoration: underline; color: rgb(0, 0, 255);");

    this->setLayoutManagement();
    this->setFixedWidth(FIXED_WIDTH);
    this->setFixedHeight(FIXED_HEIGHT);
    QObject::connect(this->btRegdirect, &QPushButton::clicked, this, &LoginScreen::slotRegdirect);
    QObject::connect(this->btLogin, &QPushButton::clicked, this, &LoginScreen::slotLogin);
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
    this->gridPane->addWidget(this->errorLabel, 5, 2);
    if(this->mc->getUserCount() > 0){
        this->gridPane->addWidget(this->btHelpdirect, 8, 0);
    }
    this->gridPane->addWidget(this->btRegdirect, 9, 0);
}
void LoginScreen::slotRegdirect(){
    //close the widget programmatically after having directed the user to the registration page
    QWidget* rsContainer = new QWidget();
    RegisterScreen* rs = new RegisterScreen(rsContainer);
    rsContainer->setLayout(rs);
    rsContainer->setFixedSize(RegisterScreen::CONTAINER_FIXED_WIDTH, RegisterScreen::CONTAINER_FIXED_HEIGHT);
    rsContainer->show();
    this->close();
}
void LoginScreen::slotLogin(){
    string givenName = this->nameTf->text().toStdString();
    string givenPassword = this->passwordTf->text().toStdString();

    if(givenName == "" || givenPassword == ""){
        this->errorLabel->setText(QString::fromStdString(LoginScreen::EMPTY_FIELD_STR));
        this->errorLabel->setVisible(true);
    }
    else if(this->mc->userExists(givenName)){
        //verify whether the given password is valid
        string password = this->mc->getPassword(givenName);
        if(password != ""){
            if(password == givenPassword){
                //grant access and deploy the mainscreen after instantiating the user
                string savecode = this->mc->getSavecode(givenName);
                User* user = new User(givenName, password, savecode);
                MainScreen* ms = new MainScreen(user);
                ms->show();
                this->close(); //programmatically close the login screen
            }
            else{
                this->errorLabel->setText(QString::fromStdString(LoginScreen::PASSWORD_MISMATCH_STR));
                this->errorLabel->setVisible(true);
            }
        }
        else{
            qDebug() << "Debug: password retrieval from the database controller was unsuccessful";
        }
    }
    else{
        this->errorLabel->setText(QString::fromStdString(LoginScreen::USER_DNE_STR));
        this->errorLabel->setVisible(true);
    }
}
void LoginScreen::slotLoginDirect(){
    //switch back to the initial login page state

    //retrieve and programmatically close the outer close's container widget if it's not the actual loginscreen object
    QObject* object = QObject::sender();
    QObject* parentObject = object->parent();

    //type cast the parentObj to QWidget then programmatically close
    QWidget* parentWidget = qobject_cast<QWidget*>(parentObject);
    if(parentWidget != this){
        parentWidget->close();
    }

    this->nameTf->clear();
    this->passwordTf->clear();
    this->setVisible(true);
}
void LoginScreen::slotHelpdirect(){
    //switch to the next phase after instantiating the layouts
    this->helpContainer1 = new QWidget();

    vector<string> usernames = this->mc->getUsernames();
    //initialize the combobox
    this->namesBox = new QComboBox(this->helpContainer1);

    cout << "Debug: slotHelpDirect usernames size is  " << usernames.size() << endl;
    for(int i = 0; i < usernames.size(); i++){
        cout << "Debug: curr username is " << usernames.at(i) << endl;
        this->namesBox->addItem(QString::fromStdString(usernames.at(i)));
    }
    this->usernameLabel = new QLabel("Select your username to reset password:", this->helpContainer1);
    this->savecodeLabel = new QLabel("Your lifetime savecode:", this->helpContainer1);
    this->savecodeTf = new QLineEdit(this->helpContainer1);
    this->btResetPass = new QPushButton("verify", this->helpContainer1);
    this->errorLabel1 = new QLabel(QString::fromStdString(LoginScreen::SAVECODE_MISMATCH_STR), this->helpContainer1);
    this->errorLabel1->setStyleSheet("color: red;");
    this->errorLabel1->setVisible(false);


    this->btLoginDirect = new QPushButton("Return login", this->helpContainer1);
    this->btLoginDirect->setStyleSheet("text-decoration: underline; color: rgb(0, 0, 255);");
    this->btLoginDirect->setFlat(true);

    this->helpPane1 = new QGridLayout(this->helpContainer1);

    this->helpPane1->addWidget(this->usernameLabel, 0, 0, 1, 2);
    this->helpPane1->addWidget(this->namesBox, 1, 0, 1, 3);
    this->helpPane1->addWidget(this->savecodeLabel, 2, 0, 1, 2);
    this->helpPane1->addWidget(this->savecodeTf, 3, 0, 1, 1);
    this->helpPane1->addWidget(this->errorLabel1, 4, 0, 1, 1);
    this->helpPane1->addWidget(this->btResetPass, 5, 3, 1, 1);
    this->helpPane1->addWidget(this->btLoginDirect, 6, 2, 1, 2);

    this->helpContainer1->setLayout(this->helpPane1);
    this->helpContainer1->show();
    this->setVisible(false);

    QObject::connect(this->btResetPass, &QPushButton::clicked, this, &LoginScreen::slotResetVerify);
    QObject::connect(this->btLoginDirect, &QPushButton::clicked, this, &LoginScreen::slotLoginDirect);
}
void LoginScreen::slotResetVerify(){
    //For simplifying implementation, we will only notify user the success of their savecode trial
    string givenSavecode = this->savecodeTf->text().toStdString();
    string username = this->namesBox->currentText().toStdString();
    string retrievedSC = this->mc->getSavecode(username);

    //cout << "Debug: retrieved savecode of the user " << username << " is " << retrievedSC << endl;
    if(givenSavecode != "" && givenSavecode == retrievedSC){
        //grant access and proceed to the next pane state
        cout << "Debug: Acces granted. Proceeding to helpPane2 state" << endl;
        this->helpContainer1->setVisible(false);
        this->savecodeTf->clear();
        this->namesBox->setCurrentIndex(0);

        this->helpContainer2 = new QWidget();
        this->helpPane2 = new QGridLayout(this->helpContainer2);
        this->expLabel2 = new QLabel("Upon successfull reset, you will be redirected to the login page", this->helpContainer2);
        this->passLabel1 = new QLabel(this->helpContainer2);
        this->passLabel2 = new QLabel(this->helpContainer2);
        this->passTf1 = new QLineEdit(this->helpContainer2);
        this->passTf2 = new QLineEdit(this->helpContainer2);
        this->errorLabel2 = new QLabel(this->helpContainer2);
        this->errorLabel2->setStyleSheet("color: red;");
        this->errorLabel2->setVisible(false);
        this->btResetAccept = new QPushButton("reset",this->helpContainer2);

        this->btReturn = new QPushButton(this->helpContainer2);
        string fileName = "iconback.jpg";
        QString iconPath = QString::fromStdString(MainScreen::ICONS_FOLDER_PATH + fileName);
        QPixmap img(iconPath);
        QRect imgRect = img.rect();
        imgRect.setSize(QSize(LoginScreen::BT_BACK_LENGTH, LoginScreen::BT_BACK_LENGTH));
        this->btReturn->setIcon(img);

        //set the layout management and display the wrapper widget
        this->helpPane2->addWidget(this->expLabel2, 0, 0);
        this->helpPane2->addWidget(this->passLabel1, 1, 0);
        this->helpPane2->addWidget(this->passTf1, 2, 0, 1, 2);
        this->helpPane2->addWidget(this->passLabel2, 3, 0);
        this->helpPane2->addWidget(this->passTf2, 4, 0, 1, 2);
        this->helpPane2->addWidget(this->errorLabel2, 5, 2);
        this->helpPane2->addWidget(this->btResetAccept, 6, 3);
        this->helpPane2->addWidget(this->btReturn, 7, 0);

        this->helpContainer2->setLayout(this->helpPane2);
        this->helpContainer2->show();

        QObject::connect(this->btResetAccept, &QPushButton::clicked, this, &LoginScreen::slotResetAccept);
        QObject::connect(this->btReturn, &QPushButton::clicked, this, &LoginScreen::slotReturnDirect);
    }
    else{
        //display the single state error label1
        this->errorLabel1->setVisible(true);
    }
}
void LoginScreen::paintEvent(QPaintEvent* event){
    QPainter painter(this); //SET THE PAINTER'S DESIGNATED TARGET
    const QRect& boundingRect = QRect(0, 0, this->width(), this->height());
    painter.drawImage(boundingRect, *this->logoImg);
}
void LoginScreen::slotResetAccept(){
    //reset the password and redirect the user to the login screen

}
void LoginScreen::slotReturnDirect(){

}
const string LoginScreen::EMPTY_FIELD_STR = "fields cannot be blank";
const string LoginScreen::USER_DNE_STR = "username does not exist";
const string LoginScreen::PASSWORD_MISMATCH_STR = "password is invalid";
const string LoginScreen::SAVECODE_MISMATCH_STR = "savecode is invalid";

