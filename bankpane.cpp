#include "bankpane.h"

using namespace std;

BankPane::BankPane(QWidget *parent)
    : AbstractPane{parent}{

    this->setFormPane();

    this->setLayout(&this->formPane);
}

void BankPane::setFormPane(){
    this->bankLabel.setText("Your bank id:");
    this->accountLabel.setText("Your account id:");
    this->btGetAccount.setText("Submit");
    this->errorLabel.setStyleSheet("color: red;");
    this->errorLabel.setVisible(false);

    this->formPane.addWidget(&this->bankLabel, 0, 0);
    this->formPane.addWidget(&this->tfBankID, 1, 0, 1, 4);
    this->formPane.addWidget(&this->accountLabel, 2, 0);
    this->formPane.addWidget(&this->tfAccountID, 3, 0, 1, 4);
    this->formPane.addWidget(&this->errorLabel);
    this->formPane.addWidget(&this->btGetAccount, 5, 3);

    QObject::connect(&this->btGetAccount, &QPushButton::clicked, this, &BankPane::slotGetAccount);
}

//Slot to initiate interaction with the OBP by attempting to get account details
void BankPane::slotGetAccount(){
    string givenAccId = this->tfAccountID.text().toStdString();
    string givenBankId = this->tfBankID.text().toStdString();

    if(givenAccId == "" || givenBankId == ""){
        this->errorLabel.setText("Fields cannot be empty");
    }
    else{

    }
}
