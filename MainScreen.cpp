#include "mainscreen.h"
#include <string>

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
    //deallocate the buttons and other controls when necessary
    for(int i = 0; i < this->toolButtons.size(); i++){
        delete toolButtons.at(i);
    }
    delete this->btLogout;
}
void MainScreen::setLayoutManagement(){
    this->hbox1 = new QHBoxLayout(this->wrapper);
    this->hbox2 = new QHBoxLayout(this->wrapper);

    this->vboxSide = new QVBoxLayout(this->wrapper);
    this->vboxScrollable = new QVBoxLayout(this->wrapper);
    this->setScrollableContent();

    this->qsa = new QScrollArea(this->wrapper);
    this->qsa->setLayout(this->vboxScrollable);
    this->qsa->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    this->qsa->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    this->vboxScrollable->addWidget(this->qsa);
    this->vboxSide->addLayout(this->vboxScrollable);
    //set the btLogout's properties after instantiating it
    this->btLogout = new QPushButton();
    QPixmap logoutImg("icons//logouticon.png");
    logoutImg.rect().setSize(QSize(LOGOUT_ICON_LENGTH, LOGOUT_ICON_LENGTH));
    this->btLogout->setIcon(logoutImg);

    this->vboxSide->addWidget(this->btLogout);

    this->addLayout(this->vboxSide); //mockup test line remove later on
    this->wrapper->setLayout(this);

    QObject::connect(this->btLogout, &QPushButton::clicked, this, &MainScreen::logoutHandler);
}
void MainScreen::setScrollableContent(){
    //initialize the pushbuttons with their icons, then add them
    for(int i = 0; i < 7; i++){
        QPushButton* currBt = new QPushButton();
        this->toolButtons.push_back(currBt);

        //set the currBt's QIcon
        char currIndex = i + 48;
        string str = "icons//icon";
        str += currIndex;
        str += ".png";
        QString iconPath = QString::fromStdString(str);
        QPixmap img(iconPath);
        img.rect().setSize(QSize(TOOL_ICON_LENGTH, TOOL_ICON_LENGTH));
        //img = img.scaled(MainScreen::TOOL_ICON_LENGTH, MainScreen::TOOL_ICON_LENGTH);
        currBt->setIcon(img);
        this->vboxScrollable->addWidget(currBt);
    }

    QPushButton* bt0 = this->toolButtons.at(0);
    QObject::connect(bt0, &QPushButton::clicked, this, &MainScreen::toolExpenseSlot);
    QPushButton* bt1 = this->toolButtons.at(1);
    QObject::connect(bt1, &QPushButton::clicked, this, &MainScreen::toolIncomeSlot);
    QPushButton* bt2 = this->toolButtons.at(2);
    QObject::connect(bt2, &QPushButton::clicked, this, &MainScreen::toolFinGoalSlot);
    QPushButton* bt3 = this->toolButtons.at(3);
    QObject::connect(bt3, &QPushButton::clicked, this, &MainScreen::toolDebtManagerSlot);
    QPushButton* bt4 = this->toolButtons.at(4);
    QObject::connect(bt4, &QPushButton::clicked, this, &MainScreen::toolVisualReportSlot);
    QPushButton* bt5 = this->toolButtons.at(5);
    QObject::connect(bt5, &QPushButton::clicked, this, &MainScreen::toolBankAccSlot);
    QPushButton* bt6 = this->toolButtons.at(6);
    QObject::connect(bt6, &QPushButton::clicked, this, &MainScreen::toolAddContactSlot);
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
void MainScreen::logoutHandler(){

}
