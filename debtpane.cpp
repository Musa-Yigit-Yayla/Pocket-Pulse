#include "debtpane.h"


DebtPane::DebtPane(User* user, QWidget* parent): AbstractPane{user, parent}{

    QObject::connect(this->btAddDebt, &QPushButton::clicked, this, &DebtPane::addDebtSlot);
}

void DebtPane::addDebtSlot(){
    QWidget* popupDebt = new QWidget();

    QPushButton* btReg = new QPushButton(popupDebt);
    popupDebt->show();
}
