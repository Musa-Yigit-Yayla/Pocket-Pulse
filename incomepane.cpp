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
        ascending = false;
    }
    else if(currSelection == this->sortChoices.at(1).toStdString()){
        sortColumn = 2;
        ascending = true;
    }
    else if(currSelection == this->sortChoices.at(2).toStdString()){
        sortColumn = 1;
        ascending = false;
    }
    else{ //if(currSelection == this->sortChoices.at(3).toStdString()){
        sortColumn = 1;
        ascending = true;
    }

    //apply heap sort
    std::make_heap(result.begin(), result.end(), ColumnComparator(sortColumn, ascending));
    std::sort_heap(result.begin(), result.end(), ColumnComparator(sortColumn, ascending));

    return result;
}
