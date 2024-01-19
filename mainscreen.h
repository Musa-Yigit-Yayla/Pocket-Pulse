#ifndef MAINSCREEN_H
#define MAINSCREEN_H

#include <maincontroller.h>
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
#include <QLayoutItem>
#include <QIcon>
#include <QColor>
#include <QPainter>
#include <QPaintDevice>

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

    QImage* topLeftImg = nullptr;
    QLabel* topLeftImgLabel = nullptr;
    vector<QToolButton*> toolButtons;
    QToolButton* btLogout = nullptr; //add widget
    QToolButton* btEditProfile = nullptr;
public:
    MainScreen(User* registeredUser); //invoke when registration has been successfully performed
    MainScreen(string username); //invoke when login is successfully performed
    ~MainScreen();

    void show(); //show the mainscreen
    void close(); //set the visiblity to false

    static const int TOOL_ICON_LENGTH = 40;
    static const int LOGOUT_ICON_LENGTH = 50;
    static const int TOP_LEFT_IMG_RADIUS = 30;
    static const QColor USER_PP_CHAR_COLOR;
    static const string ICONS_FOLDER_PATH; //change these during deployment in the deployment related branch
private:
    void setLayoutManagement();
    void setScrollableContent();
    void setProfileGrid();
public slots:
    void toolExpenseSlot();
    void toolIncomeSlot();
    void toolFinGoalSlot();
    void toolDebtManagerSlot();
    void toolVisualReportSlot();
    void toolBankAccSlot();
    void toolAddContactSlot();
    void logoutHandler();
};

#endif // MAINSCREEN_H
