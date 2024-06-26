#include "expensepane.h"
#include "user.h"
#include "maincontroller.h"
#include <QDate>
#include <QIntValidator>

using namespace std;

ExpensePane::ExpensePane(User* user, QWidget* parent): AbstractPane{user, parent}
{

    for(int i = 0; i < static_cast<int>(EXPENSE_CATEGORIES::count); i++){
        QLineEdit* lineEdit = new QLineEdit(this);
        this->textfields.push_back(lineEdit);

        QLabel* categoryLabel = new QLabel(this);
        categoryLabel->setText(QString::fromStdString(ExpensePane::CATEGORY_LABEL_STRINGS.at(i)));
        this->categoryLabels.push_back(categoryLabel);
    }
    this->dateSelector = new QComboBox(this);
    this->setCombobox();
    this->btUpdate = new QPushButton("update", this);
    this->headerLabel = new QLabel("Set your spenditure goals by specifying limits for each category for the selected month of the selected year.", this);
    this->confirmationLabel = new QLabel(this);
    this->confirmationLabel->setVisible(false);

    QFont font = this->headerLabel->font();
    font.setPointSize(8);
    this->headerLabel->setFont(font);

    this->setLayoutManagement();

    QObject::connect(this->btUpdate, &QPushButton::clicked, this, &ExpensePane::btUpdateSlot);
    QObject::connect(this, &ExpensePane::displayConfirmationLabel, this, &ExpensePane::confirmationLabelSlot);
    QObject::connect(this->dateSelector, &QComboBox::currentIndexChanged, this, &ExpensePane::fetchExistingValuesSlot);

    //manually select the first index of the dateSelector to display the placeholders
    this->dateSelector->setCurrentIndex(1);
    this->dateSelector->setCurrentIndex(0);
}
void ExpensePane::setLayoutManagement(){
    this->gridLayout = new QGridLayout(this);
    QLabel* categoryLabel = new QLabel("Category", this);
    QLabel* amountLabel = new QLabel("          Amount ($)          ", this);
    categoryLabel->setStyleSheet("text-decoration: underline;");
    amountLabel->setStyleSheet("text-decoration: underline;");
    this->gridLayout->addWidget(this->dateSelector, 0, 3);
    this->gridLayout->addWidget(categoryLabel, 1, 0);
    this->gridLayout->addWidget(amountLabel, 1, 1);

    int categoryLength =  static_cast<int>(EXPENSE_CATEGORIES::count);
    //add the category labels and textfields
    for(int i = 0; i < categoryLength; i++){
        int rowIndex = i + 2;
        QLineEdit* currTf = this->textfields.at(i);
        int textFieldWidth = 200;
        currTf->setMaximumWidth(textFieldWidth);
        QIntValidator* validator = new QIntValidator(currTf); //make the lineEdit only accept integer values
        currTf->setValidator(validator);
        this->gridLayout->addWidget(this->categoryLabels.at(i), rowIndex, 0);
        this->gridLayout->addWidget(currTf, rowIndex, 1, 1, 1);
    }
    this->btUpdate->setMaximumWidth(120);
    this->gridLayout->addWidget(this->btUpdate, categoryLength + 2, 1);
    this->gridLayout->addWidget(this->confirmationLabel, categoryLength + 3, 1);
    this->gridLayout->addWidget(this->headerLabel, categoryLength + 4, 0, 1, 4);


    this->gridLayout->setContentsMargins(100, 25, 400, 25);
    //set the layout of this pane as the gridLayout
    this->setLayout(this->gridLayout);
}
void ExpensePane::setCombobox(){
    MainController mc;
    mc.createMonthlyExpenseGoalsTable();

    //retrieve the current date and if it is the first day of the month, consider updating current available data if any exist
    QDate date = QDate::currentDate();
    QString dateStr = date.toString("dd-MM-yyyy");

    bool userGoalsDefined = mc.monthlyExpenseGoalsExist(this->user->getUserName());
    int currMonth = stoi(dateStr.toStdString().substr(3, 5));
    int currYear = stoi(dateStr.toStdString().substr(6));

    for(int i = 0; i < 12; i++){
        string currStr = ExpensePane::getMonthString(currMonth) + " " + to_string(currYear);
        this->dateSelector->addItem(QString::fromStdString(currStr));

        if(currMonth == 12){
            currMonth = 1;
            currYear++;
        }
        else{
            currMonth++;
        }
    }

}
void ExpensePane::btUpdateSlot(){
    //retrieve the combobx selection and convert it to corresponding integers
    string dateStr = this->dateSelector->currentText().toStdString();
    int blankIndex = dateStr.find(" ");

    int currMonth = getMonthInteger(dateStr.substr(0, blankIndex));
    int currYear = stoi(dateStr.substr(blankIndex + 1));

    vector<int> values;
    int categoryLength =  static_cast<int>(EXPENSE_CATEGORIES::count);
    for(int i = 0; i < categoryLength; i++){
        string currText = this->textfields.at(i)->text().toStdString();
        string placeHolder = this->textfields.at(i)->placeholderText().toStdString();
        if(currText != ""){
            values.push_back(stoi(currText));
        }
        else if(placeHolder != ""){
            //push the placeholder value to preserve the persisted value upon the new update statement
            values.push_back(stoi(placeHolder));
        }
        else{
            values.push_back(0);
        }
    }
    MainController mc;
    bool confirmSuccess = mc.registerUserMonthlyGoals(this->user->getUserName(), currMonth, currYear, values);
    emit displayConfirmationLabel(confirmSuccess);
}
void ExpensePane::confirmationLabelSlot(bool confirmed){
    QString text;
    QString colorStr;
    if(confirmed){
        text = this->confirmSuccessStr;
        colorStr = "color: rgb(0, 128, 0);";
    }
    else{
        text = this->confirmFailStr;
        colorStr = "color: rgb(255, 95, 5);";
    }
    this->confirmationLabel->setText(text);
    this->confirmationLabel->setStyleSheet(colorStr);

    this->confirmationLabel->setVisible(true);
    QTimer* timer = new QTimer(this);
    timer->setSingleShot(true);
    timer->setInterval(4000);
    QObject::connect(timer, &QTimer::timeout, this, &ExpensePane::confirmTimerSlot);
    timer->start();
}
void ExpensePane::confirmTimerSlot(){
    this->confirmationLabel->setVisible(false);
}
string ExpensePane::getMonthString(int month){
    string result = "";
    switch(month){
    case 1: result = "Jan"; break;
    case 2: result = "Feb"; break;
    case 3: result = "March"; break;
    case 4: result = "Apr"; break;
    case 5: result = "May"; break;
    case 6: result = "June"; break;
    case 7: result = "July"; break;
    case 8: result = "Aug"; break;
    case 9: result = "Sept"; break;
    case 10: result = "Oct"; break;
    case 11: result = "Nov"; break;
    case 12: result = "Dec"; break;
    }
    return result;
}
//Given string should match the month names provided by the getMonthString method
//1 based indexing
//Otherwise 0 is returned
int ExpensePane::getMonthInteger(string month){
    int result = -1;

    if(monthMap.empty()){
        //initialize the map
        monthMap.insert(make_pair("Jan", 1));
        monthMap.insert(make_pair("Feb", 2));
        monthMap.insert(make_pair("March", 3));
        monthMap.insert(make_pair("Apr", 4));
        monthMap.insert(make_pair("May", 5));
        monthMap.insert(make_pair("June", 6));
        monthMap.insert(make_pair("July", 7));
        monthMap.insert(make_pair("Aug", 8));
        monthMap.insert(make_pair("Sept", 9));
        monthMap.insert(make_pair("Oct", 10));
        monthMap.insert(make_pair("Nov", 11));
        monthMap.insert(make_pair("Dec", 12));
    }
    if(monthMap.count(month) != 0){
        result = monthMap.at(month);
    }
    return result;
}
//Event handler procedure for combobox index change event, where we try to fetch existing spenditure goal values if they exist
void ExpensePane::fetchExistingValuesSlot(int index){
    MainController mc;
    QString currSelection = this->dateSelector->currentText();
    int month = getMonthInteger(currSelection.split(" ").at(0).toStdString());
    int year = currSelection.split(" ").at(1).toInt();

    vector<int> currValues = mc.getUserMonthlyGoals(this->user->getUserName(), month, year);
    if(currValues.size() > 0){
        for(int i = 0; i < currValues.size(); i++){
            int currAmount = currValues.at(i);
            this->textfields.at(i)->setPlaceholderText(QString::fromStdString(to_string(currAmount)));
        }
    }
    else{
        for(int i = 0; i < this->textfields.size(); i++){
            //clear out the placeholders
            this->textfields.at(i)->setPlaceholderText("");
        }
    }
}
const vector<string> ExpensePane::CATEGORY_LABEL_STRINGS = {"Health", "Education", "Grocery & Market", "Entertainment", "Vehicle & Oil", "Fees", "Other"};
const string ExpensePane::MONTHLY_GOALS_TABLENAME = "MONTHLY_EXPENSE_GOALS";
unordered_map<string, int> ExpensePane::monthMap;
