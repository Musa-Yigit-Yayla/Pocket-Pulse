#include "homepane.h"

HomePane::HomePane(User* user, QWidget* parent): AbstractPane{user, parent}{
    this->refresh(); //initial invocation
}
void HomePane::refresh(){
    if(this->hbox != NULL){ //everything is instantiated
        //deallocate the datafields
        delete this->hbox;
    }
    this->hbox = new QHBoxLayout(this);
    this->vbox1 = new QVBoxLayout(this);
    this->vbox2 = new QVBoxLayout(this);

    MainController mc;

    //instantiate the debtCircle
    vector<int> debtValues = mc.getDebtStatusRatio(this->user->getUserName());
    int filledAmount = debtValues.at(0);
    int totalAmount = debtValues.at(1);

    this->debtCircle = new ProgressCircle(filledAmount, totalAmount, this);

    //instantiate the goalsCircle
    vector<int> financialGoalValues = mc.getFinancialGoalsStatusRatio(this->user->getUserName());
    filledAmount = financialGoalValues.at(0);
    totalAmount = financialGoalValues.at(1);

    this->goalsCircle = new ProgressCircle(filledAmount, totalAmount, this);

    //REORGANIZE AND ADJUST THE LAYOUT MANAGEMENT LATER ON
    this->vbox1->addWidget(this->goalsCircle);

    this->hbox->addLayout(this->vbox1);
    this->hbox->addLayout(this->vbox2);

    this->setLayout(this->hbox);

}
