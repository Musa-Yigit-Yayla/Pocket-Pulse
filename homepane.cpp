#include "homepane.h"

HomePane::HomePane(User* user, QWidget* parent): AbstractPane{user, parent}{
    this->initialSentence = QString::fromStdString(HomePane::MOTIVATION_SENTENCES.at(rand() % HomePane::MOTIVATION_SENTENCES.size()));
    this->initialSentence.replace(QRegularExpression(":username"), QString::fromStdString(this->user->getUserName()));
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

    QLabel* welcomeLabel = new QLabel(this->initialSentence);
    QFont font = welcomeLabel->font();
    const int WELCOME_FONT_SIZE = 14;
    font.setPointSize(WELCOME_FONT_SIZE);
    welcomeLabel->setFont(font);
    this->vbox1->addWidget(welcomeLabel);

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
    font = settledDebtsRatioLabel->font();
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
const vector<string> HomePane::MOTIVATION_SENTENCES = {
    "Invest in yourself, :username. It's the best investment you can make.",
    "Start saving today, :username. Your future self will thank you.",
    "Take control of your finances, :username. Don't let them control you.",
    "Live below your means, :username. It's the key to financial freedom.",
    "Make budgeting a habit, :username. It's the foundation of financial success.",
    "Don't wait to invest, :username. Time in the market beats timing the market.",
    "Compound interest is your friend, :username. Start early and watch it grow.",
    "Avoid debt whenever possible, :username. It's a roadblock to financial independence.",
    "Set clear financial goals, :username. They'll keep you focused and motivated.",
    "Learn to differentiate between wants and needs, :username. It's essential for financial well-being.",
    "Automate your savings, :username. Make it effortless to build your nest egg.",
    "Diversify your investments, :username. Don't put all your eggs in one basket.",
    "Educate yourself about money, :username. Financial literacy is the key to success.",
    "Track your spending, :username. It's the first step to financial awareness.",
    "Plan for the unexpected, :username. An emergency fund is your safety net.",
    "Don't chase after get-rich-quick schemes, :username. Slow and steady wins the race.",
    "Invest in experiences, not things, :username. Memories last a lifetime.",
    "Be patient with your investments, :username. Rome wasn't built in a day.",
    "Review your financial plan regularly, :username. Adapt it as your circumstances change.",
    "Learn from your financial mistakes, :username. They're valuable lessons in disguise.",
    "Teach your children about money, :username. It's one of the greatest gifts you can give.",
    "Celebrate your financial milestones, :username. They're proof of your progress.",
    "Don't compare your financial journey to others, :username. Focus on your own path.",
    "Be mindful of lifestyle inflation, :username. It can derail your financial goals.",
    "Take calculated risks, :username. They're necessary for growth and innovation.",
    "Invest in assets that generate passive income, :username. Let your money work for you.",
    "Stay disciplined during market downturns, :username. Keep your long-term goals in mind.",
    "Don't neglect your retirement savings, :username. Your future self deserves security.",
    "Don't be afraid to ask for financial advice, :username. Seek wisdom from those who've walked the path.",
    "Build multiple streams of income, :username. Diversification is key to financial resilience.",
    "Invest in your health, :username. Medical expenses can derail even the best financial plans.",
    "Stay informed about financial news, :username. Knowledge is power in the world of finance.",
    "Invest in assets with intrinsic value, :username. They'll withstand the test of time.",
    "Embrace frugality, :username. Small sacrifices today lead to big rewards tomorrow.",
    "Don't let fear dictate your financial decisions, :username. Rationality is your greatest ally.",
    "Negotiate for better deals, :username. It's your money; make it work harder for you.",
    "Seek out mentors in the financial world, :username. Learn from those who've achieved what you aspire to.",
    "Invest in yourself, :username. Continuous learning and self-improvement pay the highest dividends.",
    "Don't let setbacks discourage you, :username. Every obstacle is an opportunity for growth.",
    "Be proactive about your finances, :username. Don't wait for problems to arise before taking action.",
    "Learn to say no to unnecessary expenses, :username. Delayed gratification leads to lasting fulfillment.",
    "Give back to your community, :username. Generosity is a cornerstone of true wealth.",
    "Be mindful of taxes, :username. They can eat away at your returns if you're not careful.",
    "Protect your assets with insurance, :username. It's a safety net for life's uncertainties.",
    "Invest in renewable energy, :username. It's not just good for the planet; it's good for your portfolio.",
    "Plan for the long term, :username. Financial success is a marathon, not a sprint.",
    "Stay disciplined in good times and bad, :username. Consistency is the hallmark of true wealth.",
    "Invest in quality over quantity, :username. It pays off in the long run.",
    "Take calculated risks, :username. They're the engine of progress and innovation."};
