#include "mainscreen.h"
#include "user.h"
#include "bankpane.h"
#include "abstractpane.h"
#include "debtpane.h"
#include "fingoalpane.h"
#include <string>
#include <QList>
#include <iostream>
#include <QCoreApplication>

using namespace std;

//invoke when registration has been successfully performed
MainScreen::MainScreen(User* registeredUser){
    this->wrapper = new QWidget();
    this->wrapper->setLayout(this);
    this->controller = new MainController();
    this->user = registeredUser;

    //ToDo layout management
    this->setLayoutManagement();
    this->setGenericPanes();

    //set the homepane as the initial selected abstract pane
    this->toolHomeSlot();
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
    delete this->btHome;
    delete this->btLogout;
    delete this->wrapper;
}
void MainScreen::setLayoutManagement(){
    this->hbox1Widget = new QWidget(this->wrapper);
    this->hbox1 = new QHBoxLayout(this->wrapper);
    this->hbox2 = new QHBoxLayout(this->wrapper);
    this->hbox1Widget->setStyleSheet(QString::fromStdString(MainScreen::BACKGROUND_BLUE_STR));
    this->hbox2->setSpacing(5); //5 pixel spacing
    this->hbox2->setAlignment(Qt::AlignLeft);

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

    //first instantiate and add the btHome
    this->btHome = new QToolButton();
    this->btHome->setFixedSize(MainScreen::LOGOUT_ICON_LENGTH - 5, MainScreen::LOGOUT_ICON_LENGTH);
    QPixmap homeImg(QString::fromStdString(ICONS_FOLDER_PATH + "\\homeicon.png"));
    QRect imgRect = homeImg.rect();
    imgRect.setSize(QSize(LOGOUT_ICON_LENGTH, LOGOUT_ICON_LENGTH));
    this->btHome->setIcon(homeImg);
    const QString homeStr = "Home";
    this->btHome->setToolTip(homeStr);
    this->vboxScrollable->addWidget(this->btHome);

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
        currBt->setToolTip(QString::fromStdString(this->tooltips.at(i)));
        this->vboxScrollable->addWidget(currBt);
    }
    QSpacerItem* spacer1 = new QSpacerItem(*spacer0);
    this->vboxScrollable->addSpacerItem(spacer1);

    QObject::connect(this->btHome, &QToolButton::clicked, this, &MainScreen::toolHomeSlot);

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
    const int SCROLL_GAP_OFFSET = 10;
    this->qsa->setFixedWidth(LOGOUT_ICON_LENGTH + SCROLL_GAP_OFFSET);

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
void MainScreen::setGenericPanes(){

    this->bankPane = new BankPane(this->user, this->wrapper);
    this->expensePane = new ExpensePane(this->user, this->wrapper);
    this->contactsPane = new ContactsPane(this->user, this->wrapper);
    this->incomePane = new IncomePane(this->user, this->wrapper);
    this->debtPane = new DebtPane(this->user, this->wrapper);
    this->fingoalPane = new FingoalPane(this->user, this->wrapper);
    this->reportPane = new ReportPane(this->user, this->wrapper);
    this->homePane = new HomePane(this->user, this->wrapper);

    this->bankPane->setVisible(false);
    this->expensePane->setVisible(false); //modify the visibility later on
    this->contactsPane->setVisible(false);
    this->incomePane->setVisible(false);
    this->debtPane->setVisible(false);
    this->fingoalPane->setVisible(false);
    this->reportPane->setVisible(false);
    this->homePane->setVisible(false);
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
User* MainScreen::getUser() const{
    return this->user;
}
MainController* MainScreen::getMainController() const{
    return this->controller;
}
//public slots
void MainScreen::toolHomeSlot(){
    this->removeCurrGenericPane();
    this->homePane->setVisible(true);
    this->hbox2->addWidget(this->homePane);
    this->btHome->setStyleSheet("QToolButton { border: 2px solid green; }");
    //additionally refresh the homepane to fetch the latest data for progress circles and other panes
    this->homePane->refresh();
}
void MainScreen::toolExpenseSlot(){
    this->removeCurrGenericPane();
    this->expensePane->setVisible(true);
    this->hbox2->addWidget(this->expensePane);
    this->toolButtons.at(0)->setStyleSheet("QToolButton { border: 2px solid green; }");
}
void MainScreen::toolIncomeSlot(){
    this->removeCurrGenericPane();
    this->incomePane->setVisible(true);
    this->hbox2->addWidget(this->incomePane);
    this->toolButtons.at(1)->setStyleSheet("QToolButton { border: 2px solid green; }");
}
void MainScreen::toolFinGoalSlot(){
    this->removeCurrGenericPane();
    this->fingoalPane->setVisible(true);
    this->hbox2->addWidget(this->fingoalPane);
    this->toolButtons.at(2)->setStyleSheet("QToolButton { border: 2px solid green; }");
}
void MainScreen::toolDebtManagerSlot(){
    this->removeCurrGenericPane();
    this->debtPane->setVisible(true);
    this->hbox2->addWidget(this->debtPane);
    this->toolButtons.at(3)->setStyleSheet("QToolButton { border: 2px solid green; }");
}
void MainScreen::toolVisualReportSlot(){
    this->removeCurrGenericPane();
    this->reportPane->setVisible(true);
    this->hbox2->addWidget(this->reportPane);
    this->toolButtons.at(4)->setStyleSheet("QToolButton { border: 2px solid green; }");
}
void MainScreen::toolBankAccSlot(){
    this->removeCurrGenericPane();
    this->bankPane->setVisible(true);
    this->hbox2->addWidget(this->bankPane);
    this->toolButtons.at(5)->setStyleSheet("QToolButton { border: 2px solid green; }");
}
void MainScreen::toolAddContactSlot(){
    this->removeCurrGenericPane();
    this->contactsPane->setVisible(true);
    this->hbox2->addWidget(this->contactsPane);
    this->toolButtons.at(6)->setStyleSheet("QToolButton { border: 2px solid green; }");
}
inline void MainScreen::removeCurrGenericPane(){
    //QList<QObject*> children = this->hbox2->children();

    qDebug() << "Debug: hbox2 item count is " << this->hbox2->count();
    if(this->hbox2->count() == 2){
        QLayoutItem* childItem = this->hbox2->takeAt(1);
        QWidget* childWidget = childItem->widget();
        if(childWidget != NULL){
            childWidget->setVisible(false);
            childWidget->setStyleSheet("");
        }
    }
    //clear out the style from toolbuttons
    for(QToolButton* bt: this->toolButtons){
        bt->setStyleSheet("");
    }
    this->btHome->setStyleSheet("");
}
void MainScreen::logoutHandler(){
    cout << "Debug: logoutHandler has been invoked" << endl;
    QMessageBox messageBox;
    messageBox.setInformativeText(QString::fromStdString(this->user->getUserName() + ", are you sure to log out ?"));
    messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    messageBox.setDefaultButton(QMessageBox::Yes);
    int choice = messageBox.exec();

    if(choice == QMessageBox::Yes){
        this->close();
        QCoreApplication::quit();//logout
    }
}
void MainScreen::editProfileHandler(){
    //show a small widget which has two textfields for resetting the user's password
    QWidget* popupWidget = new QWidget();
    popupWidget->setFixedSize(300, 400);
    QGridLayout* passGrid = new QGridLayout(popupWidget);
    QLabel* passLabel1 = new QLabel("Password:", popupWidget);
    QLabel* passLabel2 = new QLabel("Password again:", popupWidget);
    QLineEdit* pass1 = new QLineEdit(popupWidget);
    QLineEdit* pass2 = new QLineEdit(popupWidget);
    QLabel* errLabel = new QLabel(popupWidget);
    QPushButton* btResetPass = new QPushButton("reset", popupWidget);
    errLabel->setVisible(false);

    passGrid->addWidget(passLabel1, 0, 0);
    passGrid->addWidget(pass1, 1, 0);
    passGrid->addWidget(passLabel2, 2, 0);
    passGrid->addWidget(pass2, 3, 0);
    passGrid->addWidget(errLabel, 4, 1);
    passGrid->addWidget(btResetPass, 5, 1);
    popupWidget->setLayout(passGrid);

    QObject::connect(btResetPass, &QPushButton::clicked, [this, pass1, pass2, errLabel](){
        QString text1 = pass1->text();
        QString text2 = pass2->text();
        if(text1 == "" || text2 == ""){
            errLabel->setText("Fields cannot be empty!");
            errLabel->setStyleSheet("color: rgb(255, 0, 0)");
        }
        else if(text1 != text2){
            errLabel->setText("Given passwords do not match");
            errLabel->setStyleSheet("color: rgb(255, 0, 0)");
        }
        else{
            //update the password
            MainController mc;
            bool updated = mc.updatePassword(this->user->getUserName(), text1.toStdString());
            if(updated){
                errLabel->setText("Password has been updated");
                errLabel->setStyleSheet("color: rgb(0, 255, 0);");
            }
        }
        errLabel->setVisible(true);
    });
    popupWidget->show();
}
const string MainScreen::ICONS_FOLDER_PATH = "C:\\Users\\yigit\\Desktop\\Qt_Container\\QT_PROJECTS\\Pocket-Pulse\\icons";
const QColor MainScreen::USER_PP_CHAR_COLOR = QColor(249, 166, 2);
const string MainScreen::BACKGROUND_BLUE_STR = "background-color: rgb(6, 59, 131);";
