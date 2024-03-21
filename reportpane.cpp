#include "reportpane.h"
#include "expensepane.h"
#include "bankingcontroller.h"
#include "maincontroller.h"

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

    this->vbox->addLayout(this->menuBox);

    this->initExpenseChartsPane();
    this->initIncomeExpenseDebtPane();
    this->initMonthPieChartPane();

    //!!! ADD THE INITIAL SELECTED PANE TO THE VBOX
    this->vbox->addLayout(this->monthPieChartPane);

    this->setLayout(this->vbox);
}
void ReportPane::initExpenseChartsPane(){

}
void ReportPane::initIncomeExpenseDebtPane(){

}
void ReportPane::initMonthPieChartPane(){
    this->monthPieChartPane = new QGridLayout(this);
    this->expenseDistributionChart = new PieChart(this);
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

    //then swap the elements to obtain descending order of dates
    for(int i = 0; i < spenditureMonths.size() / 2; i++){
        swap(spenditureMonths.at(i), spenditureMonths.at(spenditureMonths.size() - i - 1));
    }

    for(QString currDate: spenditureMonths){
        QString month = currDate.split("-").at(0);
        QString year = currDate.split("-").at(1);

        month = QString::fromStdString(ExpensePane::getMonthString(month.toInt()));
        this->comboBox->addItem(month + " " + year);
    }

    this->monthPieChartPane->addWidget(this->comboBox, 0, 1);
    this->monthPieChartPane->addWidget(this->expenseDistributionChart, 1, 0);

    //invoke the pieSelection slot with the last newest month to initialize pieChart contents
    //this->pieDateSelectionSlot(0);
    QObject::connect(this->comboBox, &QComboBox::currentIndexChanged, this, &ReportPane::pieDateSelectionSlot);
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
void ReportPane::pieDateSelectionSlot(int index){
    //refresh the current pie chart


    QString currItem = this->comboBox->itemText(index);

    int month = ExpensePane::getMonthInteger(currItem.split(' ').at(0).toStdString());
    int year = currItem.split(' ').at(1).toInt();

    //get the spent transactions for the given specific month and year so we can update our pie chart
    BankingController bc;
    MainController mc;
    int userID = mc.getUserId(this->user->getUserName());
    // bc.getSpentTransactions(userID, month, year, 0); //select all categories

    vector<double> transactionSums;
    vector<string> transactionHeaders = {"Health", "Education", "Market", "Entertainment", "Vehicle & Oil", "Fees", "Other"};
    int categoryCount = static_cast<int>(ExpensePane::EXPENSE_CATEGORIES::count);
    for(int i = 1; i <= categoryCount; i++){
        double currSum = bc.sumSentTransactions(userID, i, month, year);
        transactionSums.push_back(currSum);
    }
    //Proceed with updating the piechart values using the corresponding setter method
    this->expenseDistributionChart->setContents(transactionSums, transactionHeaders);

    //now simply remove the current item from the vbox so we can insert the pieChart
    //this->vbox->takeAt(1);
    //this->vbox->addLayout(this->monthPieChartPane);

}
