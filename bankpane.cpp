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

    this->formPane.addWidget(&this->bankLabel, 0, 0);
    this->formPane.addWidget(&this->tfBankID, 1, 0);
    this->formPane.addWidget(&this->accountLabel, 2, 0);
    this->formPane.addWidget(&this->tfAccountID, 3, 0);
    this->formPane.addWidget(&this->btGetAccount, 4, 3);

    QObject::connect(&this->btGetAccount, &QPushButton::clicked, this, &BankPane::slotGetAccount);
}

void BankPane::slotGetAccount(){

}
