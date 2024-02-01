#include "abstractpane.h"

AbstractPane::AbstractPane(User* user, QWidget* parent): QWidget{parent}{
    this->user = user;
}
AbstractPane::~AbstractPane(){

}
