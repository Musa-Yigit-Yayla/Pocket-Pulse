#include "mainscreen.h"

using namespace std;

//invoke when registration has been successfully performed
MainScreen::MainScreen(User* registeredUser){
    this->wrapper = new QWidget();
    this->wrapper->setLayout(this);
    this->controller = new MainController();
    this->user = registeredUser;

    //ToDo layout management
    this->setLayoutManagement();
}
//invoke when login is successfully performed
MainScreen::MainScreen(string username){
    this->wrapper = new QWidget();
    this->wrapper->setLayout(this);
    this->controller = new MainController();
}
//invoke when login is successfully performed
MainScreen::~MainScreen(){
    //close the controller by deallocation
    delete this->controller;
    delete this->wrapper;
    if(this->user != NULL){
        //delete the user object
        delete this->user;
    }
}
void MainScreen::setLayoutManagement(){
    this->wrapper = new QWidget();
    this->hbox1 = new QHBoxLayout(this->wrapper);
    this->hbox2 = new QHBoxLayout(this->wrapper);

    this->vboxSide = new QVBoxLayout(this->wrapper);
    this->vboxScrollable = new QVBoxLayout(this->wrapper);
    this->setScrollableContent();

    this->qsa = new QScrollArea(this->wrapper);
}
void MainScreen::setScrollableContent(){
    //initialize the pushbuttons with their icons, then add them
    QPushButton* toolExpense = new QPushButton();
    this->toolButtons.push_back(toolExpense);

    QObject::connect(toolExpense, &QPushButton::clicked, this, &MainScreen::toolExpenseSlot);
}
void MainScreen::show(){
    if(this->wrapper->isHidden()){
        this->wrapper->show();
    }
    this->wrapper->setVisible(true);
}
//set the visiblity of the container QWidget to false
void MainScreen::close(){
    this->wrapper->setVisible(false);
}

//public slots
void MainScreen::toolExpenseSlot(){

}
void MainScreen::toolIncomeSlot(){

}
void MainScreen::toolFinGoalSlot(){

}
void MainScreen::toolDebtManagerSlot(){

}
void MainScreen::toolVisualReportSlot(){

}
void MainScreen::toolBankAccSlot(){

}
void MainScreen::toolAddContactSlot(){

}
