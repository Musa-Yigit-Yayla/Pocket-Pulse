#include "fingoalpane.h"
#include "maincontroller.h"
#include "bankingcontroller.h"

FingoalPane::FingoalPane(User* user, QWidget* parent): AbstractPane{user, parent}{
    //retrieve the date of today
    QDate currTime = QDateTime::currentDateTime().date();
    int month = currTime.month();
    int year = currTime.year();

    //retrieve the financial goals registered for this month
    MainController mc;
    BankingController bc;
    vector<int> spenditureGoals = mc.getUserMonthlyGoals(this->user->getUserName(), month, year);
    int userId = mc.getUserId(this->user->getUserName());
    vector<vector<QVariant>> transactions = bc.getSpentTransactions(userId, month, year); //all columns are returned

    this->setTransactionsGrid(transactions);
    this->setRectGrid(spenditureGoals);

}
void FingoalPane::setRectGrid(vector<int>& spenditureGoals){
    //allocate the rectangles and fill the rect array
    BankingController bc;
    MainController mc;

    double spenditureGoalsSum = 0;
    double transactionsSum = 0;
    for(int i = 0; i < this->RECTS_LENGTH; i++){
        double currFillRatio;
        ProgressRectangle* currRect = nullptr;
        if(i != 7){ //the regular case of calculating transactions wrt their category
            int userId = mc.getUserId(this->user->getUserName());
            double currSum = bc.sumSentTransactions(userId, i);
            double currGoal = spenditureGoals.at(i);
            currFillRatio = currGoal / currSum; //i stands for the current category of sent transactions
            currRect = new ProgressRectangle(RECT_WIDTH, RECT_HEIGHT, currFillRatio, this);
            transactionsSum += currSum;
            spenditureGoalsSum += currGoal;
        }
        else{
            //the special case where we instantiate the total sum rectangle
            if(spenditureGoalsSum == 0){
                currFillRatio = 0;
            }
            else{
                currFillRatio = transactionsSum / spenditureGoalsSum;
            }
            currRect = new ProgressRectangle(RECT_WIDTH, RECT_HEIGHT, currFillRatio, this);
        }
        this->spenditureRects[i] = currRect;
    }
    this->redrawRectangles();
}
void FingoalPane::setTransactionsGrid(vector<vector<QVariant>>& transactions){
    //initialize the contents of the combobox
    QList<QString> categories = {"All", "Health", "Education", "Market/Grocery", "Entertainment", "Vehicle & Petrol", "Fees", "Other"}; //all specifies select all transactions
    this->cbTransactionCategory->addItems(QStringList::fromVector(categories));

    QLabel* expLabel = new QLabel("Sent Transactions", this);
    expLabel->setStyleSheet("font-size: 16px;");
    this->transactionsGrid->addWidget(expLabel, 0, 0);
    this->transactionsGrid->addWidget(this->cbTransactionCategory, 0, 1);


    QObject::connect(this->cbTransactionCategory, &QComboBox::currentIndexChanged, this, &FingoalPane::cbTransactionSlot);
    //since initial selection is All, display each and every transaction in the given vector sorted by their dates
    this->cbTransactionCategory->setCurrentIndex(0);
}
/*FingoalPane::~FingoalPane(){
    //deallocate dynamically allocated rectangles and the container array
    for(int i = 0; i < this->RECTS_LENGTH; i++){
        delete this->spenditureRects[i];
    }
    delete[] this->spenditureRects;
}*/
void FingoalPane::redrawRectangles(){

}
//Draw the given rectangle to display progress of the attribıute represented by it
//successRatio must be a floating point number [0, 1]. 0 indicates no progress (full green), 1 indicates full capacity (full red)
void FingoalPane::paintProgressRect(QRect& rect, double successRatio){

}
void FingoalPane::cbTransactionSlot(int index){

}
