#include "fingoalpane.h"

FingoalPane::FingoalPane(User* user, QWidget* parent): AbstractPane{user, parent}{
    //allocate the rectangles and fill the rect array
    for(int i = 0; i < this->RECTS_LENGTH; i++){
        QRect* newRect = new QRect();
        newRect->setSize(QSize(this->RECT_WIDTH, this->RECT_HEIGHT));
        //this->spenditureRects[i] = new ProgressRectangle(this->RECT_WIDTH, this->RECT_HEIGHT);
    }
    this->redrawRectangles();
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
