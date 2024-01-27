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
    this->close();
    rsContainer->show();
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

    this->passLabel1 = new QLabel("Password");
    this->passLabel2 = new QLabel("Password again");
    //this->passTf1 = new QLineEdit();
    //this->passTf2 = new QLineEdit(this->helpContainer1);*/

    QLineEdit* le1 = new QLineEdit();
    le1->setEchoMode(QLineEdit::Password);
    QLineEdit* le2 = new QLineEdit();
    le1->setEchoMode(QLineEdit::Password);

    this->passLabels.push_back(le1);
    this->passLabels.push_back(le2);


    this->helpPane1 = new QGridLayout(this->helpContainer1);

    this->helpPane1->addWidget(this->usernameLabel, 0, 0, 1, 2);
    this->helpPane1->addWidget(this->namesBox, 1, 0, 1, 3);
    this->helpPane1->addWidget(this->savecodeLabel, 2, 0, 1, 2);
    this->helpPane1->addWidget(this->savecodeTf, 3, 0, 1, 1);
    this->helpPane1->addWidget(this->errorLabel1, 4, 0, 1, 1);
    this->helpPane1->addWidget(this->btResetPass, 5, 3, 1, 1);
    this->helpPane1->addWidget(this->btLoginDirect, 6, 2, 1, 2);
    this->helpPane1->addWidget(this->passLabel1, 7, 0, 1, 2);
    this->helpPane1->addWidget(le1, 8, 0, 1, 1);
    this->helpPane1->addWidget(this->passLabel2, 9, 0, 1, 2);
    this->helpPane1->addWidget(le2, 10, 0, 1, 1);



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
        qDebug() << "Debug: given savecode is valid executing password validation";
        if(this->errorLabel1->text().toStdString() == SAVECODE_MISMATCH_STR){
            this->errorLabel1->setVisible(false);
        }
        //Retrieve the textfields from the container, then validate the given password content on both textfields
        //QLineEdit* passField1 = qobject_cast<QLineEdit*>(this->helpContainer1->childAt(8, 0));
        //QLineEdit* passField2 = qobject_cast<QLineEdit*>(this->helpContainer1->childAt(10, 0));

        qDebug() << "Debug: password retrieval executed successfully";

        //string pass1 = passField1->text().toStdString();
        //string pass2 = passField2->text().toStdString();

        if(this->passLabels.at(0) == NULL || this->passLabels.at(1) == NULL){
            cout << "Debug: passFields are nullptr" << endl;
        }
        else{
            cout << "Debug: passFields are not nullptr" << endl;
        }
        string pass1 = this->passLabels.at(0)->text().toStdString();
        string pass2 = this->passLabels.at(1)->text().toStdString();

        if(pass1 != pass2){
            this->errorLabel1->setVisible(true);
            this->errorLabel1->setText("Passwords do not match");
        }
        else if(!RegisterScreen::isValidPassword(pass1)){
            this->errorLabel1->setVisible(true);
            this->errorLabel1->setText("A password must be at least length 8 and\n contain lowercase and uppercase letter(s) \n digit(s) and special character(s)");
        }
        else{
            //given password is valid, update the database column
            bool success = this->mc->updatePassword(username, pass1);

            qDebug() << "Debug: Success of password reset operation is " << success;
            if(success){
                this->errorLabel1->setText("Password resetted successfuly\n you will be redirected to login shortly");
                this->errorLabel1->setStyleSheet("color: green;");
                this->errorLabel1->setVisible(true);

                QTimer* redirectTimer = new QTimer(); //set the parent later on when applicable

            }

        }


       /* //grant access and proceed to the next pane state
        cout << "Debug: Acces granted. Proceeding to helpPane2 state" << endl;
        //delete this->helpContainer1;
        //this->helpContainer1 = nullptr;
        //this->savecodeTf->clear();
        //this->namesBox->setCurrentIndex(0);

        this->helpContainer2 = new QWidget();
        //this->expLabel2 = new QLabel("Upon successfull reset\n you will be redirected to the login page");
        this->passLabel1 = new QLabel("Password");
        this->passLabel2 = new QLabel("Password again");
        this->passTf1 = new QLineEdit();
        //this->passTf2 = new QLineEdit();
        //this->errorLabel2 = new QLabel();
        //this->errorLabel2->setStyleSheet("color: red;");
        //this->errorLabel2->setVisible(false);
        //this->btResetAccept = new QPushButton("confirm");

        /*this->btReturn = new QPushButton();
        string fileName = "\\icon2.jpg";
        QString iconPath = QString::fromStdString(MainScreen::ICONS_FOLDER_PATH + fileName);
        QPixmap img(iconPath);
        QRect imgRect = img.rect();
        imgRect.setSize(QSize(LoginScreen::BT_BACK_LENGTH, LoginScreen::BT_BACK_LENGTH));
        this->btReturn->setIcon(img);

        this->helpPane2 = new QGridLayout();
        //set the layout management and display the wrapper widget
        //this->helpPane2->addWidget(this->expLabel2, 0, 0, 1, 1);
        this->helpPane2->addWidget(this->passLabel1, 1, 0, 1, 1);
        //this->helpPane2->addWidget(this->passTf1, 2, 0);
        //this->helpPane2->addWidget(this->passLabel2, 3, 0, 1, 1);
        //this->helpPane2->addWidget(this->passTf2, 4, 0, 1, 2);
        //this->helpPane2->addWidget(this->errorLabel2, 5, 2, 1, 1);
        //this->helpPane2->addWidget(this->btResetAccept, 6, 3, 1, 1);
        //this->helpPane2->addWidget(this->btReturn, 7, 0, 1, 1);

        this->helpContainer2->setLayout(this->helpPane2);
        this->helpContainer2->show();

        QObject::connect(this->btResetAccept, &QPushButton::clicked, this, &LoginScreen::slotResetAccept);
        QObject::connect(this->btReturn, &QPushButton::clicked, this, &LoginScreen::slotReturnDirect);
        /*QPushButton* bt = new QPushButton("hi");
        QVBoxLayout* vb = new QVBoxLayout();
        vb->addWidget(bt);
        this->helpContainer2->setLayout(vb);
        this->helpContainer2->show();*/
    }
    else{
        this->errorLabel1->setText("Given savecode is invalid");
        this->errorLabel1->setVisible(true);
    }
    cout << "Debug: reached end of execution for LoginScreen::slotResetVerify" << endl;
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

