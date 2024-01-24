#include "mainscreen.h"
#include "user.h"
#include <string>
#include <iostream>

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
    //!!! YOU MIGHT WANT TO SAVE THE CURRENT STATUS OF THE USER TO THE DATABASE BEFORE DEALLOCATION
    //close the controller by deallocation
    delete this->controller;
    if(this->user != NULL){
        //delete the user object
        delete this->user;
    }
    //deallocate the buttons and other controls when necessary
    for(int i = 0; i < this->toolButtons.size(); i++){
        delete toolButtons.at(i);
    }
    delete this->btLogout;
    delete this->wrapper;
}
void MainScreen::setLayoutManagement(){
    this->hbox1Widget = new QWidget(this->wrapper);
    this->hbox1 = new QHBoxLayout(this->wrapper);
    this->hbox2 = new QHBoxLayout(this->wrapper);
    this->hbox1Widget->setStyleSheet(QString::fromStdString(MainScreen::BACKGROUND_BLUE_STR));

    this->vboxSide = new QVBoxLayout(this->wrapper);
    this->vboxScrollable = new QVBoxLayout(this->wrapper);
    this->vboxScrollable->setSpacing(0);
    this->setScrollableContent();

    this->profileGrid = new QGridLayout(this->wrapper);
    this->setProfileGrid();
    this->hbox1->addLayout(this->profileGrid);

    this->animatedLabel = new AnimatedLabel(this->wrapper);
    this->setAnimatedLabel();
    this->hbox1->addWidget(this->animatedLabel);
    this->hbox1Widget->setLayout(this->hbox1);

    this->addWidget(this->hbox1Widget);
    this->hbox2->addLayout(this->vboxSide);
    this->addLayout(this->hbox2);
    this->wrapper->setLayout(this);
}
void MainScreen::setScrollableContent(){
    QSpacerItem* spacer0 = new QSpacerItem(50, 50);
    this->vboxScrollable->addSpacerItem(spacer0);
    //initialize the pushbuttons with their icons, then add them
    for(int i = 0; i < 7; i++){
        QToolButton* currBt = new QToolButton();
        currBt->setFixedSize(TOOL_ICON_LENGTH, TOOL_ICON_LENGTH);
        this->toolButtons.push_back(currBt);

        //set the currBt's QIcon
        char currIndex = i + 48;
        string str = "\\icon";
        str += currIndex;
        str += ".png";
        QString iconPath = QString::fromStdString(MainScreen::ICONS_FOLDER_PATH + str);
        QPixmap img(iconPath);
        QRect imgRect = img.rect();
        imgRect.setSize(QSize(TOOL_ICON_LENGTH, TOOL_ICON_LENGTH));
        //img = img.scaled(MainScreen::TOOL_ICON_LENGTH, MainScreen::TOOL_ICON_LENGTH);
        currBt->setIcon(img);
        this->vboxScrollable->addWidget(currBt);
    }
    QSpacerItem* spacer1 = new QSpacerItem(*spacer0);
    this->vboxScrollable->addSpacerItem(spacer1);

    QToolButton* bt0 = this->toolButtons.at(0);
    QObject::connect(bt0, &QToolButton::clicked, this, &MainScreen::toolExpenseSlot);
    QToolButton* bt1 = this->toolButtons.at(1);
    QObject::connect(bt1, &QToolButton::clicked, this, &MainScreen::toolIncomeSlot);
    QToolButton* bt2 = this->toolButtons.at(2);
    QObject::connect(bt2, &QToolButton::clicked, this, &MainScreen::toolFinGoalSlot);
    QToolButton* bt3 = this->toolButtons.at(3);
    QObject::connect(bt3, &QToolButton::clicked, this, &MainScreen::toolDebtManagerSlot);
    QToolButton* bt4 = this->toolButtons.at(4);
    QObject::connect(bt4, &QToolButton::clicked, this, &MainScreen::toolVisualReportSlot);
    QToolButton* bt5 = this->toolButtons.at(5);
    QObject::connect(bt5, &QToolButton::clicked, this, &MainScreen::toolBankAccSlot);
    QToolButton* bt6 = this->toolButtons.at(6);
    QObject::connect(bt6, &QToolButton::clicked, this, &MainScreen::toolAddContactSlot);

    //set the scrollable area
    this->qsa = new QScrollArea(this->wrapper);
    this->qsa->setLayout(this->vboxScrollable);
    this->qsa->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    this->qsa->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    this->vboxSide->addWidget(this->qsa);
    //set the btLogout's properties after instantiating it
    this->btLogout = new QToolButton();
    QPixmap logoutImg(QString::fromStdString(MainScreen::ICONS_FOLDER_PATH + "\\iconlogout.png"));
    //logoutImg.rect().setSize(QSize(LOGOUT_ICON_LENGTH, LOGOUT_ICON_LENGTH));
    this->btLogout->setIcon(logoutImg);
    this->btLogout->setFixedSize(LOGOUT_ICON_LENGTH, LOGOUT_ICON_LENGTH);

    this->vboxSide->addWidget(this->btLogout);

    //this->addLayout(this->vboxSide); //mockup test line remove later on

    QObject::connect(this->btLogout, &QToolButton::clicked, this, &MainScreen::logoutHandler);
}
void MainScreen::setProfileGrid(){
    string imagePath = this->user->getProfileImagePath();
    this->topLeftImg = new QImage();

    //QPainter painter;
    //painter.begin(this->topLeftImg);
    if(imagePath != "" && this->topLeftImg->load(QString::fromStdString(imagePath))){
        //given profile image has been loaded successfully
        this->topLeftImgLabel = new QLabel(this->wrapper);
        this->topLeftImgLabel->setPixmap(QPixmap::fromImage(*topLeftImg));
        this->profileGrid->addWidget(this->topLeftImgLabel, 0, 0);
    }
    else{
        //paint the topLeftImg with the user's initial char by using CircularImage class
        cout << "Debug: about to instantiate the CircularImage class" << endl;
        //delete and reinstantiate the topLeftImg
        delete this->topLeftImg;
        this->topLeftImg = nullptr;

        this->ppInitialImage = new CircularImage(this->user->getUserName().at(0));
        this->profileGrid->addWidget(this->ppInitialImage, 0, 0);
    }
    //painter.drawArc()

    QLabel* nameLabel = new QLabel(QString::fromStdString(this->user->getUserName()), this->wrapper);
    nameLabel->setStyleSheet("color: rgb(249, 246, 238);");
    this->profileGrid->addWidget(nameLabel, 0, 1);
    this->btEditProfile = new QToolButton();
    QPixmap editImg(QString::fromStdString(MainScreen::ICONS_FOLDER_PATH) + "\\editiconwhite.png");
    this->btEditProfile->setIcon(editImg);
    this->profileGrid->addWidget(this->btEditProfile);

    QObject::connect(this->btEditProfile, &QToolButton::clicked, this, &MainScreen::editProfileHandler);
}
void MainScreen::setAnimatedLabel(){
    //set the animated label properties and initialize the animation
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
    cout << "Debug: logoutHandler has been invoked" << endl;
}
void MainScreen::editProfileHandler(){

}
const string MainScreen::ICONS_FOLDER_PATH = "C:\\Users\\yigit\\Desktop\\Qt_Container\\QT_PROJECTS\\Pocket-Pulse\\icons";
const QColor MainScreen::USER_PP_CHAR_COLOR = QColor(249, 166, 2);
const string MainScreen::BACKGROUND_BLUE_STR = "background-color: rgb(6, 59, 131);";
