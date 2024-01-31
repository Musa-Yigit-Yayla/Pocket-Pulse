#include "bankpane.h"
#include "bankingcontroller.h"

using namespace std;

BankPane::BankPane(QWidget *parent)
    : AbstractPane{parent}{

    this->setFormPane();

    this->setLayout(&this->formPane);
}

void BankPane::setFormPane(){
    this->accountLabel.setText("Your account id:");
    this->passLabel.setText("Your account password:");
    this->btGetAccount.setText("Submit");
    this->errorLabel.setStyleSheet("color: red;");
    this->errorLabel.setVisible(false);

    this->formPane.addWidget(&this->accountLabel, 0, 0);
    this->formPane.addWidget(&this->tfAccountID, 1, 0, 1, 4);
    this->formPane.addWidget(&this->passLabel, 2, 0);
    this->formPane.addWidget(&this->tfAccPass, 3, 0, 1, 4);
    this->formPane.addWidget(&this->errorLabel);
    this->formPane.addWidget(&this->btGetAccount, 5, 3);

    QObject::connect(&this->btGetAccount, &QPushButton::clicked, this, &BankPane::slotGetAccount);
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
                   //provide authorizationwith the account
                   this->errorLabel.setVisible(false);
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
