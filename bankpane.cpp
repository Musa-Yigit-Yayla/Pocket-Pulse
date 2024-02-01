#include "bankpane.h"
#include "bankingcontroller.h"
#include "mainscreen.h"

using namespace std;

BankPane::BankPane(User* user, QWidget *parent)
    : AbstractPane{user, parent}{

    this->setFormPane();
    this->setLayoutManagement();
    //this->setLayout(&this->formPane);
}

void BankPane::setFormPane(){
    this->accountLabel.setText("Your account id:");
    this->passLabel.setText("Your account password:");
    this->btGetAccount.setText("Submit");
    this->errorLabel.setStyleSheet("color: red;");
    this->errorLabel.setVisible(false);
    this->tfAccPass.setEchoMode(QLineEdit::Password);

    this->formPane.addWidget(&this->accountLabel, 0, 0);
    this->formPane.addWidget(&this->tfAccountID, 1, 0, 1, 4);
    this->formPane.addWidget(&this->passLabel, 2, 0);
    this->formPane.addWidget(&this->tfAccPass, 3, 0, 1, 4);
    this->formPane.addWidget(&this->errorLabel);
    this->formPane.addWidget(&this->btGetAccount, 5, 3);

    QObject::connect(&this->btGetAccount, &QPushButton::clicked, this, &BankPane::slotGetAccount);
}
void BankPane::setLayoutManagement(){
    this->pane = new QHBoxLayout(this);
    this->accountBox = new QVBoxLayout(this);



    this->pane->addLayout(this->accountBox);
    this->pane->addLayout(&this->formPane);

}
//Slot to initiate interaction with the OBP by attempting to get account details
void BankPane::slotGetAccount(){
    string givenAccId = this->tfAccountID.text().toStdString();
    string givenPass = this->tfAccPass.text().toStdString();

    if(givenAccId == "" || givenAccId == ""){
        this->errorLabel.setText("Fields cannot be empty");
    }
    else{
        //try to locate the account with given id if it exists, then validate the given password
        int id;
        bool idCasted = false;
        try{
           id = stoi(givenAccId);
            idCasted = true;
        }
        catch(exception){
           this->errorLabel.setText("Given account id must be an integer");
           this->errorLabel.setVisible(true);
        }
        if(idCasted){
           BankingController bc;

           if(bc.accountExists(id)){
               string password = bc.getAccountAttribute(id, BankingController::ACCOUNT_ATTRIBUTES::BANK_PASSWORD);
               if(givenPass == password){
                   //check whether specified account id is already in use by some user
                   if(bc.accountRegistered(id)){
                       this->errorLabel.setText(QString::fromStdString("Specified account with id " + givenAccId + " is in use"));
                       this->errorLabel.setVisible(true);
                   }
                   else{
                       //provide authorization with the account and add the account
                       QWidget* mainScreenWrapper = qobject_cast<QWidget*>(this->parent());
                       MainScreen* ms = nullptr;
                       MainController* mainController = nullptr;
                       int userId = -1;
                       if(mainScreenWrapper != NULL){
                           ms = reinterpret_cast<MainScreen*>(mainScreenWrapper->layout());
                       }
                       if(ms != NULL){
                           mainController = ms->getMainController();
                       }
                       if(mainController != NULL){
                           userId = mainController->getUserId(ms->getUser()->getUserName());
                       }
                       else{
                           qDebug() << "Debug: error occured during casting of mainscreen and its wrapper at function BankPane::slotAccount";
                       }
                       if(userId != -1){
                           //register the account to the relational table for user has a relation
                           bool registered = bc.registerAccountToUser(id, userId);
                           qDebug() << "Debug: user account registration yielded " << registered;
                           this->errorLabel.setVisible(false);
                       }
                   }

               }
               else{
                   this->errorLabel.setText("Given password is invalid");
                   this->errorLabel.setVisible(true);
               }
           }
           else{
               this->errorLabel.setText("No such account exists with the given id");
               this->errorLabel.setVisible(true);
           }
        }
    }
}
void BankPane::viewTransactions(){

}
