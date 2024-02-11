#include "expensepane.h"
#include "user.h"
#include "maincontroller.h"
#include <QDate>

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


}
void ExpensePane::setLayoutManagement(){

}
void ExpensePane::setCombobox(){
    MainController mc;
    mc.createMonthlyExpenseGoalsTable();

    //retrieve the current date and if it is the first day of the month, consider updating current available data if any exist
    QDate date = QDate::currentDate();
    QString dateStr = date.toString("dd-MM-yyyy");

    bool userGoalsDefined = mc.monthlyExpenseGoalsExist(this->user->getUserName());
    if(!userGoalsDefined){
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
    else{
        int day = stoi(dateStr.toStdString().substr(0, 2));
        if(day == 1){
            //create a new month entry for the corresponding user in addition to the latest 11 months if necessary
        }
    }

}
void ExpensePane::btUpdateSlot(){

}
inline string ExpensePane::getMonthString(int month){
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
const vector<string> ExpensePane::CATEGORY_LABEL_STRINGS = {"Health", "Education", "Grocery & Market", "Entertainment", "Vehicle & Oil", "Fees", "Other"};
const string ExpensePane::MONTHLY_GOALS_TABLENAME = "MONTHLY_EXPENSE_GOALS";
