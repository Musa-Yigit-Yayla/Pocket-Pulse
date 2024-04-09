#ifndef MAINSCREEN_H
#define MAINSCREEN_H

#include "maincontroller.h"
#include "circularimage.h"
#include "animatedlabel.h"
#include "bankpane.h"
#include "expensepane.h"
#include "contactspane.h";
#include "incomepane.h"
#include "debtpane.h"
#include "fingoalpane.h"
#include "reportpane.h"
#include "homepane.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QScrollArea>
#include <QWidget>
#include <QString>
#include <QImage>
#include <QRect>
#include <cstddef>
#include <string>
#include <vector>
#include <QPushButton>
#include <QToolButton>
#include <QToolTip>
#include <QLayoutItem>
#include <QIcon>
#include <QColor>
#include <QPainter>
#include <QPaintDevice>
#include <QMessageBox>

using namespace std;
class MainScreen: public QVBoxLayout{
private:
    QWidget* wrapper = nullptr;
    MainController* controller = nullptr;
    User* user = nullptr;

    QHBoxLayout* hbox1 = nullptr;
    QHBoxLayout* hbox2 = nullptr;
    QVBoxLayout* vboxSide = nullptr; //vbox for containing menu scrollpane and logout icon
    QVBoxLayout* vboxScrollable = nullptr;
    QScrollArea* qsa = nullptr;
    QWidget* tbWidget = nullptr; //widget for holding toolbuttons
    QGridLayout* profileGrid = nullptr;
    QWidget* hbox1Widget = nullptr;

    QImage* topLeftImg = nullptr;
    CircularImage* ppInitialImage = nullptr;
    QLabel* topLeftImgLabel = nullptr;
    AnimatedLabel* animatedLabel = nullptr;

    QToolButton* btHome = nullptr;
    vector<QToolButton*> toolButtons;
    QToolButton* btLogout = nullptr; //add widget
    QToolButton* btEditProfile = nullptr;

    //generic pane datafields
    HomePane* homePane = nullptr;
    BankPane* bankPane = nullptr;
    ExpensePane* expensePane = nullptr;
    ContactsPane* contactsPane = nullptr;
    IncomePane* incomePane = nullptr;
    DebtPane* debtPane = nullptr;
    FingoalPane* fingoalPane = nullptr;
    ReportPane* reportPane = nullptr;

    vector<string> tooltips = {"Manage expenses", "Track income", "Financial goals", "Debt management", "View reports", "Bank account", "Manage contacts"};
public:
    MainScreen(User* registeredUser); //invoke when registration has been successfully performed
    MainScreen(string username); //invoke when login is successfully performed
    ~MainScreen();

    void show(); //show the mainscreen
    void close(); //set the visiblity to false
    User* getUser() const;
    MainController* getMainController() const;

    static const int TOOL_ICON_LENGTH = 40;
    static const int LOGOUT_ICON_LENGTH = 50;
    static const int TOP_LEFT_IMG_RADIUS = 30;
    static const QColor USER_PP_CHAR_COLOR;
    static const string ICONS_FOLDER_PATH; //change these during deployment in the deployment related branch
    static const string BACKGROUND_BLUE_STR;
private:
    void setLayoutManagement();
    void setScrollableContent();
    void setProfileGrid();
    void setAnimatedLabel();
    void setGenericPanes(); //method for initializing the generic pane datafields
    inline void removeCurrGenericPane();
public slots:
    void toolHomeSlot();
    void toolExpenseSlot();
    void toolIncomeSlot();
    void toolFinGoalSlot();
    void toolDebtManagerSlot();
    void toolVisualReportSlot();
    void toolBankAccSlot();
    void toolAddContactSlot();
    void logoutHandler();
    void editProfileHandler();
};

#endif // MAINSCREEN_H
