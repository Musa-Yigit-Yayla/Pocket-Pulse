#include "reportpane.h"
#include "expensepane.h"
#include "bankingcontroller.h"
#include "maincontroller.h"
#include <cmath>

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

    this->initGoalsChartPane();
    this->initIncomeExpenseDebtPane();
    this->initMonthPieChartPane();

    //!!! ADD THE INITIAL SELECTED PANE TO THE VBOX
    //this->vbox->addLayout(this->monthPieChartPane);
    //this->chartWrapper->setLayout(this->monthPieChartPane);
    this->vbox->addWidget(this->chartWrapper);

    this->setLayout(this->vbox);
    //invoke the combobox currIndexChanges (for piecharts) manually
    if(this->comboBox->count() >= 2){
        this->comboBox->setCurrentIndex(1);
        this->comboBox->setCurrentIndex(0);
    }
    if(this->cbGoalDate->count() >= 2){
        this->cbGoalDate->setCurrentIndex(1);
        this->cbGoalDate->setCurrentIndex(0);
    }

    QObject::connect(this->tbECP, &QToolButton::clicked, this, &ReportPane::menuSelectionSlot);
    QObject::connect(this->tbIEDP, &QToolButton::clicked, this, &ReportPane::menuSelectionSlot);
    QObject::connect(this->tbMPCP, &QToolButton::clicked, this, &ReportPane::menuSelectionSlot);
}
void ReportPane::initGoalsChartPane(){
    this->goalsChartPane = new QGridLayout();
    this->goalDistributionChart = new PieChart();

    //retrieve the months in which the user has specified any financial goals
    MainController mc;
    vector<vector<int>> monthsWithGoals = mc.getMonthsWithExpenseGoals(this->user->getUserName());

    //initialize the related combobox
    for(vector<int> currDate: monthsWithGoals){
        int year = currDate.at(1);
        int month = currDate.at(0);
        string monthStr = ExpensePane::getMonthString(month);
        this->cbGoalDate->addItem(QString::fromStdString(to_string(year) + " " + monthStr));
    }

    this->goalsChartPane->addWidget(this->cbGoalDate, 0, 1);
    this->goalsChartPane->addWidget(this->goalDistributionChart, 1, 0);

    QObject::connect(this->cbGoalDate, &QComboBox::currentIndexChanged, this, &ReportPane::pieDateSelectionSlot);
}
void ReportPane::initIncomeExpenseDebtPane(){
    this->incomeExpenseDebtPane = new QGridLayout();
    this->rectGrid = new QGridLayout();

    //initialize layout management for this pane
    this->controlWrapper->addWidget(this->fromComboBox);
    this->controlWrapper->addWidget(this->toComboBox);
    this->controlWrapper->addWidget(this->dateAllCheckBox);
    this->controlWrapper->addWidget(this->btRefresh);


    this->dateAllCheckBox->setChecked(true);
    this->fromComboBox->setVisible(false);
    this->toComboBox->setVisible(false);

    int minMonth;
    int maxMonth;
    int minYear;
    int maxYear;
    //initialize the contents of the combobox
    //select the whole range
    //use aggregate min max functions, in the controller methods you will implement, in SQL
    MainController mc;
    vector<int> dateSpanGoals = mc.getMaxExpenseGoalSpan(this->user->getUserName());

    int monthMinExpense = INT_MAX;
    int yearMinExpense = INT_MAX;
    int monthMaxExpense = INT_MIN;
    int yearMaxExpense = INT_MIN;

    if(dateSpanGoals.size() == 4){
        monthMinExpense = dateSpanGoals.at(0);
        yearMinExpense = dateSpanGoals.at(1);
        monthMaxExpense = dateSpanGoals.at(2);
        yearMaxExpense = dateSpanGoals.at(3);
    }
    //retrieve the max and min of participated transactions
    BankingController bc;
    vector<int> dateSpanTransactions = bc.getMaxTransactionsDateSpan(this->user->getUserName());
    int monthMinTransaction = INT_MAX;
    int yearMinTransaction = INT_MAX;
    int monthMaxTransaction = INT_MIN;
    int yearMaxTransaction = INT_MIN;

    if(dateSpanTransactions.size() == 4){
        monthMinTransaction = dateSpanTransactions.at(0);
        yearMinTransaction = dateSpanTransactions.at(1);
        monthMaxTransaction = dateSpanTransactions.at(2);
        yearMaxTransaction = dateSpanTransactions.at(3);
    }
    minMonth = min(monthMinExpense, monthMinTransaction);
    minYear = min(yearMinExpense, yearMinTransaction);
    maxMonth = max(monthMaxExpense, monthMaxTransaction);
    maxYear = max(yearMaxExpense, yearMaxTransaction);
    if(minMonth != INT_MAX){ //checking only one condition guarantees the existence of others
        //add each and every month from min month and year to max month and year for simplicity even if no entry is found, into the combobox
        int currMonth = minMonth, currYear = minYear;
        while(currYear < maxYear || (currYear == maxYear && currMonth <= maxMonth)){
            //add the curr item
            string monthStr = ExpensePane::getMonthString(currMonth);
            QString dateStr = QString::fromStdString(monthStr + " " + to_string(currYear));
            this->fromComboBox->addItem(dateStr);
            this->toComboBox->addItem(dateStr);
            if(currMonth == 12){
                currMonth = 1;
                currYear++;
            }
            else{
                currMonth++;
            }
        }
    }
    //manually invoke the barchart drawing slot
    this->barChartRedrawSlot(0);

    QObject::connect(this->fromComboBox, &QComboBox::currentIndexChanged, this, &ReportPane::barChartRedrawSlot);
    QObject::connect(this->toComboBox, &QComboBox::currentIndexChanged, this, &ReportPane::barChartRedrawSlot);
    QObject::connect(this->dateAllCheckBox, &QCheckBox::stateChanged, this, &ReportPane::barChartRedrawSlot);
    QObject::connect(this->btRefresh, &QPushButton::clicked, this, &ReportPane::barChartRedrawSlot);
}
void ReportPane::initMonthPieChartPane(){
    this->monthPieChartPane = new QGridLayout();
    this->expenseDistributionChart = new PieChart();
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
//Returns a visual element wrapped into a widget, representing visual data in barchart form regarding sent/received transactions and total spenditure goal
//Returns nullptr if no entry has been found to visualize
QWidget* ReportPane::getMonthBarChart(int month, int year){
    QWidget* widget = nullptr;
    const double RECT_MAX_HEIGHT = 350;
    const double RECT_WIDTH = 35;

    MainController mc;
    vector<int> spenditureGoals = mc.getUserMonthlyGoals(this->user->getUserName(), month, year);
    int spenditureGoalsSum = 0;
    for(int currGoal: spenditureGoals){
        spenditureGoalsSum += currGoal;
    }
    BankingController bc;
    vector<int> transactions = bc.getMonthlyTransactionsFromInterval(this->user->getUserName(), month, year); //if no entry is found this vector contains two zeros
    int receivedTransactionsSum = transactions.at(0);
    int sentTransactionsSum = transactions.at(1);

    if(spenditureGoalsSum != 0 || receivedTransactionsSum != 0 || sentTransactionsSum != 0){
        widget = new QWidget();
        QGridLayout* grid = new QGridLayout(widget);
        QVBoxLayout* rectLabelBox = new QVBoxLayout(widget);
        QHBoxLayout* rectBox = new QHBoxLayout(widget);
        rectBox->setSpacing(0);

        //proceed by initializing the rectangles and labels then complete layout management
        QLabel* dateLabel = new QLabel(QString::fromStdString(ExpensePane::getMonthString(month) + " " + to_string(year)), widget);
        QFont font = dateLabel->font();
        font.setBold(true);
        dateLabel->setFont(font);

        double maxSum = max(spenditureGoalsSum, max(receivedTransactionsSum, sentTransactionsSum));
        int incomeHeight = (receivedTransactionsSum * RECT_MAX_HEIGHT) / maxSum;
        int sentHeight = (sentTransactionsSum * RECT_MAX_HEIGHT) / maxSum;
        int goalHeight = (spenditureGoalsSum * RECT_MAX_HEIGHT) / maxSum;

        RectWidget* rectIncome = new RectWidget(RECT_WIDTH, incomeHeight, *(const_cast<QColor*>(&ReportPane::RECEIVED_TRANSACTION_COLOR)), widget);
        RectWidget* rectSpenditure = new RectWidget(RECT_WIDTH, sentHeight, *(const_cast<QColor*>(&ReportPane::SENT_TRANSACTION_COLOR)), widget);
        RectWidget* rectGoals = new RectWidget(RECT_WIDTH, goalHeight, *(const_cast<QColor*>(&ReportPane::TOTAL_SPENDITURE_GOAL_COLOR)), widget);
        rectBox->addWidget(rectIncome);
        rectBox->addWidget(rectSpenditure);
        rectBox->addWidget(rectGoals);

        QLabel* incomeLabel = new QLabel(QString::fromStdString("Income: " + to_string(receivedTransactionsSum) + "$"), widget);
        QLabel* spenditureLabel = new QLabel(QString::fromStdString("Spenditure: " + to_string(sentTransactionsSum) + "$"), widget);
        QLabel* goalLabel = new QLabel(QString::fromStdString("Spenditure Goals: " + to_string(spenditureGoalsSum) + "$"), widget);

        rectLabelBox->addLayout(rectBox);
        rectLabelBox->addWidget(incomeLabel);
        rectLabelBox->addWidget(spenditureLabel);
        rectLabelBox->addWidget(goalLabel);

        grid->addWidget(dateLabel, 0, 1);
        grid->addLayout(rectLabelBox, 0, 0);
        widget->setLayout(grid);
    }
    return widget;
}
void ReportPane::menuSelectionSlot(){
    QObject* eventSource = QObject::sender();
    qDebug() << "Debug: ReportPane::menuSelectionSlot invoked";
    this->vbox->takeAt(1); //remove the chartWrapper from its parent initially
    this->chartWrapper->setLayout(nullptr); //remove the currently set layout
    //delete this->chartWrapper;
    this->chartWrapper = new QWidget();
    //QLayoutItem* takenItem = this->vbox->takeAt(1);
    /*qDebug() << "Debug: takenItem in menuSelectionSlot is " << takenItem;
    qDebug() << "Debug: takenItem == monthPieChartPane yields " << (takenItem == this->monthPieChartPane);
    qDebug() << "Debug: takenItem == goalsChartPane yields " << (takenItem == this->goalsChartPane);*/
    qDebug() << "Debug: currVBox item count before adding the selected layout is " << this->vbox->count();

    if(eventSource == this->tbMPCP){
        this->chartWrapper->setLayout(this->monthPieChartPane);
        qDebug() << "Debug: branched into tbMPCP if block";
    }
    else if(eventSource == this->tbIEDP){
        this->chartWrapper->setLayout(this->incomeExpenseDebtPane);
        qDebug() << "Debug: branched into tbIEDP if block";
    }
    else if(eventSource == this->tbECP){
        this->chartWrapper->setLayout(this->goalsChartPane);
        qDebug() << "Debug: branched into tbECP if block";
    }
    this->vbox->addWidget(this->chartWrapper);
}
void ReportPane::pieDateSelectionSlot(int index){
    QObject* eventSource = QObject::sender();

    if(eventSource == this->comboBox){
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
        //this->vbox->addLayout(this->monthcPieChartPane);
    }
    else if(eventSource == this->cbGoalDate){
        //refresh the pie chart regarding goal distributions
        QString currItem = this->cbGoalDate->itemText(index);

        int month = ExpensePane::getMonthInteger(currItem.split(' ').at(1).toStdString());
        int year = currItem.split(' ').at(0).toInt();

        MainController mc;

        vector<int> goals = mc.getUserMonthlyGoals(this->user->getUserName(), month, year);
        vector<double> goalsDouble;
        //convert goals to a double vector
        for(int i = 0; i < goals.size(); i++){
            double curr = goals.at(i);
            goalsDouble.push_back(curr);
        }
        vector<string> goalHeaders = ExpensePane::CATEGORY_LABEL_STRINGS;

        qDebug() << "Debug: goalsDouble array has contents " << goalsDouble << " and size " << goalsDouble.size();
        this->goalDistributionChart->setContents(goalsDouble, goalHeaders);
    }
}
//for the date comboboxes of iedPane and its pushbuttons and the checkbox
void ReportPane::barChartRedrawSlot(int index){
    //The main approach is to retrieve income, expense, and total spenditure goal limit sums and display it on a monthly basis
    //on the given time interval (inclusively on endpoints). If the given time interval is set to all using checkbox
    //select the widest range in which any of the sums exist for the current user
    if(this->fromComboBox->count() > 0 && this->toComboBox->count() > 0){
        QString fromDate;
        QString toDate;
        if(this->dateAllCheckBox->isChecked()){
            //select the first and last entry in one of the comboboxes
            fromDate = this->fromComboBox->itemText(0);
            toDate = this->toComboBox->itemText(this->toComboBox->count() - 1);
        }
        else{
            fromDate = this->fromComboBox->currentText();
            toDate = this->toComboBox->currentText();
        }
        int fromMonth = ExpensePane::getMonthInteger(fromDate.split(' ').at(0).toStdString());
        int fromYear = fromDate.split(' ').at(1).toInt();
        int toMonth = ExpensePane::getMonthInteger(toDate.split(' ').at(0).toStdString());
        int toYear = toDate.split(' ').at(1).toInt();

        int currMonth = fromMonth;
        int currYear = fromYear;

        int currRow = 0;
        int currColumn = 0;
        const int columnCount = 8;
        while(currYear < toYear || (currYear == toYear && currMonth <= toMonth)){
            QWidget* monthBarChart = this->getMonthBarChart(currMonth, currYear);
            if(monthBarChart != NULL){
                //insert into the rectGrid
                this->rectGrid->addWidget(monthBarChart, currRow, currColumn);
                if(currColumn + 1 == columnCount){
                    currRow++;
                    currColumn = 0;
                }
                else{
                    currColumn++;
                }
            }
            if(currMonth == 12){
                currMonth = 1;
                currYear++;
            }
            else{
                currMonth++;
            }
        }
    }
    //vector<vector<int>> getMonthlyTransactionsFromInterval(this->user->getUserName(), fromDate, toDate);
}
const QColor ReportPane::SENT_TRANSACTION_COLOR(232, 14, 2);
const QColor ReportPane::RECEIVED_TRANSACTION_COLOR(5, 248, 8);
const QColor ReportPane::TOTAL_SPENDITURE_GOAL_COLOR(232, 140, 2);
