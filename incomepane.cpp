#include "incomepane.h"
#include "bankingcontroller.h"
#include <algorithm>

using namespace std;

struct ColumnComparator{
    int column;
    bool ascending;

    ColumnComparator(int colIndex, bool ascending = false): column{colIndex}, ascending{ascending}{

    }
    //Perform operator overloading with signature () for providing a functor
    bool operator()(const vector<int>& v1, const vector<int>& v2) const{
        bool result = v1.at(column) < v2.at(column);
        if(ascending){
            result = v1.at(column) > v2.at(column);
        }
        return result;
    }
};

IncomePane::IncomePane(User* user, QWidget* parent): AbstractPane{user, parent}{
    this->grid = new QGridLayout(this);
    this->btRefresh = new QPushButton("Refresh");
    this->cbSortSelector = new QComboBox();
    this->cbSortSelector->addItems(this->sortChoices);
    this->cbIgnoreDate = new QCheckBox("Ignore date boundary");
    this->cbIgnoreDate->setChecked(true);

    this->grid->addWidget(this->cbSortSelector);
    this->displayIncome();
    this->grid->addWidget(this->btRefresh, 2, 2);

    QObject::connect(this->btRefresh, &QPushButton::clicked, this, &IncomePane::refreshHandler);
}
//Redraw the vbox by deallocationg and reordering with respect to the combobox selection
void IncomePane::displayIncome(){
    if(this->vbox != NULL && this->sa != NULL){
        delete this->vbox;
        delete this->sa;

        this->vbox = nullptr;
        this->sa = nullptr;
    }
    this->vbox = new QVBoxLayout(this);
    this->sa = new QScrollArea(this);
    this->sa->setWidgetResizable(true);


    vector<vector<int>> sortedData = this->getSortedIncome();

    for(int i = 0; i < sortedData.size(); i++){
        vector<int> dataRow = sortedData.at(i);
        QHBoxLayout* currRow = this->getIncomeRow(dataRow);
        this->vbox->addLayout(currRow);
    }
    this->sa->setLayout(vbox);

    //!!!The below code might be unnecessary, verify later
    this->grid->addWidget(this->sa, 1, 0);

}
void IncomePane::refreshHandler(){
    this->displayIncome();
}
//Uses heap sort methods of algorithm library
vector<vector<int>> IncomePane::getSortedIncome(){
    BankingController bc;
    vector<vector<int>> result = bc.retrieveIncome(this->user->getUserName());

    int sortColumn;
    bool ascending; //larger items first when ascending is false

    string currSelection = this->cbSortSelector->currentText().toStdString();
    if(currSelection == this->sortChoices.at(0).toStdString()){
        //most recent
        sortColumn = 2;
        ascending = true;
    }
    else if(currSelection == this->sortChoices.at(1).toStdString()){
        sortColumn = 2;
        ascending = false;
    }
    else if(currSelection == this->sortChoices.at(2).toStdString()){
        sortColumn = 1;
        ascending = true;
    }
    else{ //if(currSelection == this->sortChoices.at(3).toStdString()){
        sortColumn = 1;
        ascending = false;
    }

    //apply heap sort
    std::make_heap(result.begin(), result.end(), ColumnComparator(sortColumn, ascending));
    std::sort_heap(result.begin(), result.end(), ColumnComparator(sortColumn, ascending));

    return result;
}
QHBoxLayout* IncomePane::getIncomeRow(vector<int>& data){
    QHBoxLayout* hbox = new QHBoxLayout();

    BankingController bc;
    string senderName = bc.getAccountAttribute(data.at(0), BankingController::ACCOUNT_ATTRIBUTES::ID);

    QLabel* senderLabel = new QLabel(QString::fromStdString(senderName), this->sa);
    QLabel* amountLabel= new QLabel(QString::fromStdString("$" + to_string(data.at(1))), this->sa);
    QDate date = BankingController::fromDate.addDays(data.at(2));
    QLabel* dateLabel = new QLabel(date.toString(Qt::DateFormat::ISODate), this->sa);

    hbox->addWidget(senderLabel);
    hbox->addWidget(amountLabel);
    hbox->addWidget(dateLabel);

    return hbox;
}



