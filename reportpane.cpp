#include "reportpane.h"
#include "expensepane.h"
#include "bankingcontroller.h"

using namespace std;


ReportPane::ReportPane(User* user, QWidget* parent): AbstractPane{user, parent}{
    //initialize the menubox by placing toolbuttons into it

    tbECP->setText(this->toolButtonStrings[0]);
    tbIEDP->setText(this->toolButtonStrings[1]);
    tbMPCP->setText(this->toolButtonStrings[2]);

    //initially tbECP option is selected, so mark its stroke to green in CSS
    tbECP->setStyleSheet("stroke-color: green;");

    this->menuBox->addWidget(tbECP);
    this->menuBox->addWidget(tbIEDP);
    this->menuBox->addWidget(tbMPCP);
    this->menuBox->setSpacing(20);

    this->initExpenseChartsPane();
    this->initIncomeExpenseDebtPane();
    this->initMonthPieChartPane();

    this->vbox->addLayout(this->menuBox);
    //!!! ADD THE INITIAL SELECTED PANE TO THE VBOX

    this->setLayout(this->vbox);
}
void ReportPane::initExpenseChartsPane(){

}
void ReportPane::initIncomeExpenseDebtPane(){

}
void ReportPane::initMonthPieChartPane(){
    this->monthPieChartPane = new QGridLayout(this);
    //set the combobox content to the months along with years in which user has made spending transactions using any of their registered accounts
    BankingController bc;
    vector<QString> spenditureMonths = bc.getSpenditureMonths(this->user->getUserName()); //returned values are in the form of month-year, hence convert it to the string form

    //first sort the spenditureMonths using bubble sort
    for(int i = 0; i < spenditureMonths.size() - 1; i++){
        for(int j = 0; j < spenditureMonths.size() - i - 1; j++){
            QString currStr = spenditureMonths.at(j);
            QString nextStr = spenditureMonths.at(j + 1);
            int currVal = currStr.split("-").at(0).toInt() + 12 * currStr.split("-").at(1).toInt();
            int nextVal = nextStr.split("-").at(0).toInt() + 12 * nextStr.split("-").at(1).toInt();

            if(currVal > nextVal){
                swap(spenditureMonths.at(j), spenditureMonths.at(j + 1));
            }
        }
    }

    for(QString currDate: spenditureMonths){
        QString month = currDate.split("-").at(0);
        QString year = currDate.split("-").at(1);

        month = QString::fromStdString(ExpensePane::getMonthString(month.toInt()));
        this->comboBox->addItem(month + " " + year);
    }

    this->monthPieChartPane->addWidget(this->comboBox, 0, 1);
}
void ReportPane::menuSelectionSlot(){
    QObject* eventSource = QObject::sender();
    this->vbox->takeAt(1);
    if(eventSource == this->tbMPCP){
        this->vbox->addLayout(this->monthPieChartPane);
    }
    else if(eventSource == this->tbIEDP){
        this->vbox->addLayout(this->incomeExpenseDebtPane);
    }
    else if(eventSource == this->tbECP){
        this->vbox->addLayout(this->expenseChartsPane);
    }
}
