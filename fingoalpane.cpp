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
    this->setRectGrid(transactions, spenditureGoals);

}
void FingoalPane::setRectGrid(vector<vector<QVariant>>& transactions, vector<int>& spenditureGoals){
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
            currFillRatio = bc.sumSentTransactions(userId, i); //i stands for the current category of sent transactions
            currRect = new ProgressRectangle(RECT_WIDTH, RECT_HEIGHT, currFillRatio);
        }
        else{
            //the special case where we instantiate the total sum rectangle
        }

    }
    this->redrawRectangles();
}
void FingoalPane::setTransactionsGrid(vector<vector<QVariant>>& transactions){

}
FingoalPane::~FingoalPane(){
    //deallocate dynamically allocated rectangles and the container array
    for(int i = 0; i < this->RECTS_LENGTH; i++){
        delete this->spenditureRects[i];
    }
    delete[] this->spenditureRects;
}
void FingoalPane::redrawRectangles(){

}
//Draw the given rectangle to display progress of the attribıute represented by it
//successRatio must be a floating point number [0, 1]. 0 indicates no progress (full green), 1 indicates full capacity (full red)
void FingoalPane::paintProgressRect(QRect& rect, double successRatio){

}
