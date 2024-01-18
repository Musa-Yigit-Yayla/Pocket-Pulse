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
    //deallocate the buttons and other controls when necessary
}
void MainScreen::setLayoutManagement(){
    this->wrapper = new QWidget();
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
    QPixmap logoutImg("icons/logouticon.png");
    logoutImg.rect().setSize(QSize(LOGOUT_ICON_LENGTH, LOGOUT_ICON_LENGTH));
    this->btLogout->setIcon(logoutImg);

    this->vboxSide->addWidget(this->btLogout);

    QObject::connect(this->btLogout, &QPushButton::clicked, this, &MainScreen::logoutHandler);
}
void MainScreen::setScrollableContent(){
    //initialize the pushbuttons with their icons, then add them
    for(int i = 0; i < 7; i++){
        QPushButton* currBt = new QPushButton();

        //set the currBt's QIcon
        string str = "icons/icon" + to_string(i) + ".png";
        QString iconPath = QString::fromStdString(str);
        QPixmap img(iconPath);
        img.rect().setSize(QSize(TOOL_ICON_LENGTH, TOOL_ICON_LENGTH));
        //img = img.scaled(MainScreen::TOOL_ICON_LENGTH, MainScreen::TOOL_ICON_LENGTH);
        currBt->setIcon(img);
        this->vboxScrollable->addWidget(currBt);
    }

    QObject::connect(reinterpret_cast<QPushButton*>(this->vboxScrollable->itemAt(0)), &QPushButton::clicked, this, &MainScreen::toolExpenseSlot);
    QObject::connect(reinterpret_cast<QPushButton*>(this->vboxScrollable->itemAt(1)), &QPushButton::clicked, this, &MainScreen::toolIncomeSlot);
    QObject::connect(reinterpret_cast<QPushButton*>(this->vboxScrollable->itemAt(2)), &QPushButton::clicked, this, &MainScreen::toolFinGoalSlot);
    QObject::connect(reinterpret_cast<QPushButton*>(this->vboxScrollable->itemAt(3)), &QPushButton::clicked, this, &MainScreen::toolDebtManagerSlot);
    QObject::connect(reinterpret_cast<QPushButton*>(this->vboxScrollable->itemAt(4)), &QPushButton::clicked, this, &MainScreen::toolVisualReportSlot);
    QObject::connect(reinterpret_cast<QPushButton*>(this->vboxScrollable->itemAt(5)), &QPushButton::clicked, this, &MainScreen::toolBankAccSlot);
    QObject::connect(reinterpret_cast<QPushButton*>(this->vboxScrollable->itemAt(6)), &QPushButton::clicked, this, &MainScreen::toolAddContactSlot);
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
