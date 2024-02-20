#include "incomepane.h"
#include "bankingcontroller.h"

IncomePane::IncomePane(User* user, QWidget* parent): AbstractPane{user, parent}{
    this->grid = new QGridLayout(this);
    this->btRefresh = new QPushButton("Refresh");
    this->cbSortSelector = new QComboBox(this);
    this->cbSortSelector->addItems(this->sortChoices);
    this->cbIgnoreDate = new QCheckBox("Ignore date boundary", this);
    this->cbIgnoreDate->setChecked(true);

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

    vector<vector<int>> sortedData = this->getSortedIncome();
}
void IncomePane::refreshHandler(){
    this->displayIncome();
}
vector<vector<int>> IncomePane::getSortedIncome(){
    BankingController bc;
    vector<vector<int>> result = bc.retrieveIncome(this->user->getUserName(), this->cbSortSelector->currentText());
}
