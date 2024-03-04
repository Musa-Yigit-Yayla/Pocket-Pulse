#include "fingoalpane.h"
#include "maincontroller.h"

FingoalPane::FingoalPane(User* user, QWidget* parent): AbstractPane{user, parent}{
    //retrieve the date of today
    QDate currTime = QDateTime::currentDateTime().date();
    int month = currTime.month();
    int year = currTime.year();

    //retrieve the financial goals registered for this month
    MainController mc;
    vector<int> spenditures = mc.getUserMonthlyGoals(this->user->getUserName(), month, year);
    this->setTransactionsGrid();

    //allocate the rectangles and fill the rect array
    for(int i = 0; i < this->RECTS_LENGTH; i++){
        QRect* newRect = new QRect();
        newRect->setSize(QSize(this->RECT_WIDTH, this->RECT_HEIGHT));
        //this->spenditureRects[i] = new ProgressRectangle(this->RECT_WIDTH, this->RECT_HEIGHT);
    }
    this->redrawRectangles();
}
void FingoalPane::setRectGrid(){

}
void FingoalPane::setTransactionsGrid(){

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
