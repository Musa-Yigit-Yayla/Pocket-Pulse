#include "homepane.h"

HomePane::HomePane(User* user, QWidget* parent): AbstractPane{user, parent}{
    this->refresh(); //initial invocation
}
void HomePane::refresh(){
    if(this->hbox != NULL){ //everything is instantiated
        //deallocate the datafields
        delete this->goalsCircle;
        delete this->debtCircle;
        delete this->debtSA;
        delete this->goalSA;
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
    const int RATIO_LABEL_FONT_SİZE = 18;

    QLabel* settledDebtsRatioLabel = new QLabel("Settled Debts Ratio", this);
    QFont font = settledDebtsRatioLabel->font();
    font.setPointSize(RATIO_LABEL_FONT_SİZE);
    settledDebtsRatioLabel->setFont(font);
    settledDebtsRatioLabel->setStyleSheet("color: rgb(255, 140, 0)");

    QLabel* achievedGoalsRatioLabel = new QLabel("Achieved Financial Goals Ratio", this);
    font = achievedGoalsRatioLabel->font();
    font.setPointSize(RATIO_LABEL_FONT_SİZE);
    achievedGoalsRatioLabel->setFont(font);
    achievedGoalsRatioLabel->setStyleSheet("color: rgb(65, 105, 225)");

    QHBoxLayout* hboxCircleWrapper = new QHBoxLayout(this);

    QVBoxLayout* vboxCircleWrapper1 = new QVBoxLayout(this);
    vboxCircleWrapper1->addWidget(settledDebtsRatioLabel);
    vboxCircleWrapper1->addWidget(this->debtCircle);

    QVBoxLayout* vboxCircleWrapper2 = new QVBoxLayout(this);
    vboxCircleWrapper2->addWidget(achievedGoalsRatioLabel);
    vboxCircleWrapper2->addWidget(this->goalsCircle);

    hboxCircleWrapper->addLayout(vboxCircleWrapper1);
    hboxCircleWrapper->addLayout(vboxCircleWrapper2);

    //this->vbox1->addWidget(settledDebtsRatioLabel);
    this->vbox1->addLayout(hboxCircleWrapper);

    //instantiate the scrollpanes and their contents
    this->debtSA = new QScrollArea(this);
    QVBoxLayout* debtVBox = new QVBoxLayout(this->debtSA);
    vector<vector<QVariant>> debts = mc.getAllDebts(this->user->getUserName(), false);

    const int HEADER_LABEL_FONT_SIZE = 14;
    QLabel* debtLabel = new QLabel("Paid Debts", this->debtSA);
    font = debtLabel->font();
    font.setPointSize(HEADER_LABEL_FONT_SIZE);
    debtLabel->setFont(font);
    debtLabel->setStyleSheet("color: rgb(255, 140, 0);");
    debtVBox->addWidget(debtLabel);

    //display only the paid debts
    //6th index is debt paid status
    int debtStatusIndex = 6;
    const int DEBT_HBOX_SPACING = 20;
    for(int i = 0; i < debts.size(); i++){
        vector<QVariant> currDebt = debts.at(i);
        if(currDebt.at(debtStatusIndex).toInt() == 1){
            //append the current paid debt into the debtVBox
            QHBoxLayout* debtHBox = new QHBoxLayout(this->debtSA);
            debtHBox->setSpacing(DEBT_HBOX_SPACING);
            QLabel* owedNameLabel = new QLabel(currDebt.at(2).toString());
            QLabel* amountLabel = new QLabel(currDebt.at(3).toString() + "$");
            QLabel* expLabel = new QLabel(currDebt.at(4).toString());
            QLabel* dateLabel = new QLabel(currDebt.at(5).toString());

            debtHBox->addWidget(owedNameLabel);
            debtHBox->addWidget(amountLabel);
            debtHBox->addWidget(expLabel);
            debtHBox->addWidget(dateLabel);
            debtVBox->addLayout(debtHBox);
        }
    }
    QWidget* debtWrapper = new QWidget(this->debtSA);
    //this->debtSA->setMaximumHeight(500);
    debtWrapper->setLayout(debtVBox);

    this->debtSA->setWidget(debtWrapper);
    this->vbox1->addWidget(this->debtSA);

    //instantiate and configure attained financial goals
    this->goalSA = new QScrollArea(this);
    QVBoxLayout* goalVBox = new QVBoxLayout(this->goalSA);
    QLabel* goalLabel = new QLabel("Attained Financial Goals", this->goalSA);
    font = goalLabel->font();
    font.setPointSize(RATIO_LABEL_FONT_SİZE);
    goalLabel->setFont(font);
    goalLabel->setStyleSheet("color: rgb(65, 105, 225);");
    goalVBox->addWidget(goalLabel);

    vector<vector<QVariant>> financialGoals = mc.getFullFinancialGoals(this->user->getUserName());
    int goalStatusIndex = 4;
    for(int i = 0; i < financialGoals.size(); i++){
        vector<QVariant> currGoal = financialGoals.at(i);
        if(currGoal.at(goalStatusIndex).toInt() == 1){
            QHBoxLayout* goalHBox = new QHBoxLayout(this->goalSA);
            goalHBox->setSpacing(DEBT_HBOX_SPACING);
            QLabel* expLabel = new QLabel(currGoal.at(2).toString());
            QLabel* dateLabel = new QLabel(currGoal.at(3).toString());

            goalHBox->addWidget(expLabel);
            goalHBox->addWidget(dateLabel);
            goalVBox->addLayout(goalHBox);
        }
    }
    QWidget* goalWrapper = new QWidget(this->goalSA);


    goalWrapper->setLayout(goalVBox);
    this->goalSA->setWidget(goalWrapper);
    this->vbox2->addWidget(this->goalSA);

    this->hbox->addLayout(this->vbox1);
    this->hbox->addLayout(this->vbox2);

    this->setLayout(this->hbox);

}
