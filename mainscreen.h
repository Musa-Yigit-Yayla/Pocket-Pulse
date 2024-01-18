#ifndef MAINSCREEN_H
#define MAINSCREEN_H

#include <maincontroller.h>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QWidget>
#include <QString>
#include <QImage>
#include <cstddef>
#include <string>
#include <vector>
#include <QPushButton>
#include <QLayoutItem>
#include <QIcon>

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

    vector<QPushButton*> toolButtons;
public:
    MainScreen(User* registeredUser); //invoke when registration has been successfully performed
    MainScreen(string username); //invoke when login is successfully performed
    ~MainScreen();

    void show(); //show the mainscreen
    void close(); //set the visiblity to false
    void setLayoutManagement();
    void setScrollableContent();

    static const int TOOL_ICON_LENGTH = 40;
public slots:
    void toolExpenseSlot();
    void toolIncomeSlot();
    void toolFinGoalSlot();
    void toolDebtManagerSlot();
    void toolVisualReportSlot();
    void toolBankAccSlot();
    void toolAddContactSlot();
};

#endif // MAINSCREEN_H
