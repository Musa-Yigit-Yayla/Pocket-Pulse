#include "homepane.h"

HomePane::HomePane(User* user, QWidget* parent): AbstractPane{user, parent}{

}
void HomePane::refresh(){
    if(this->hbox != NULL){ //everything is instantiated
        //deallocate the datafields
        delete this->hbox;
    }
    this->hbox = new QHBoxLayout(this);
    this->vbox1 = new QVBoxLayout(this);
    this->vbox2 = new QVBoxLayout(this);

    //instantiate the debtCircle
    int filledAmount;
    int totalAmount;

    this->debtCircle = new ProgressCircle(filledAmount, totalAmount);
}
