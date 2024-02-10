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

    }
    else{
        try{
            int day = stoi(dateStr.toStdString().substr(0, 2));
            if(day == 1){
                //create a new table for the corresponding user
            }
        }
        catch(exception){

        }
    }

}
void ExpensePane::btUpdateSlot(){

}
const vector<string> ExpensePane::CATEGORY_LABEL_STRINGS = {"Health", "Education", "Grocery & Market", "Entertainment", "Vehicle & Oil", "Fees", "Other"};
const string ExpensePane::MONTHLY_GOALS_TABLENAME = "MONTHLY_EXPENSE_GOALS";
