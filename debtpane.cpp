#include "debtpane.h"
#include "maincontroller.h"


DebtPane::DebtPane(User* user, QWidget* parent): AbstractPane{user, parent}{

    QObject::connect(this->btAddDebt, &QPushButton::clicked, this, &DebtPane::addDebtSlot);
}

void DebtPane::addDebtSlot(){
    QWidget* popupDebt = new QWidget();

    QLineEdit* tfOwedName = new QLineEdit(popupDebt);
    QLineEdit* tfAmount = new QLineEdit(popupDebt);
    QLineEdit* tfExplanation = new QLineEdit(popupDebt);
    QComboBox* cbContactName = new QComboBox(popupDebt);
    cbContactName->addItem("-");

    //retrieve the contact names
    MainController mc;
    string userName = this->user->getUserName();
    vector<vector<QString>> contactAttributes = mc.retrieveContacts(userName);
    for(int i = 0; i < contactAttributes.size(); i++){
        cbContactName->addItem(contactAttributes.at(i).at(0));
    }

    QPushButton* btReg = new QPushButton(popupDebt);


    popupDebt->show();
}
