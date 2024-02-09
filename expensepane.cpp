#include "expensepane.h"
#include "user.h"

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


}
void ExpensePane::setLayoutManagement(){

}
void ExpensePane::btUpdateSlot(){

}
const vector<string> ExpensePane::CATEGORY_LABEL_STRINGS = {"Health", "Education", "Grocery & Market", "Entertainment", "Vehicle & Oil", "Fees", "Other"};
