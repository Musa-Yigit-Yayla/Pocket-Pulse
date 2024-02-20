#include "incomepane.h"

IncomePane::IncomePane(User* user, QWidget* parent): AbstractPane{user, parent}{
    this->grid = new QGridLayout(this);
    this->btRefresh = new QPushButton("Refresh");
    this->cbSortSelector = new QComboBox(this);
    this->cbSortSelector->addItems({"Most recent", "Oldest" , "Max amount", "Min amount"});

    QObject::connect(this->btRefresh, &QPushButton::clicked, this, &IncomePane::refreshHandler);
}
//Redraw the vbox by deallocationg and reordering with respect to the combobox selection
void IncomePane::displayIncome(){
    if(this->vbox != NULL && this->sa != NULL){
        delete this->vbox;
        delete this->sa;
    }
    this->vbox = new QVBoxLayout(this);
    this->sa = new QScrollArea(this);
    this->sa->setWidgetResizable(true);


}
void IncomePane::refreshHandler(){
    this->displayIncome();
}
