#include "mainscreen.h"

using namespace std;

//invoke when registration has been successfully performed
MainScreen::MainScreen(User* registeredUser){
    this->wrapper = new QWidget();
    this->wrapper->setLayout(this);
    this->controller = new MainController();
    this->user = registeredUser;

    //ToDo layout management
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
