#ifndef MAINSCREEN_H
#define MAINSCREEN_H

#include <maincontroller.h>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QString>
#include <cstddef>
#include <string>

using namespace std;
class MainScreen: public QVBoxLayout{
private:
    QWidget* wrapper = nullptr;
    MainController* controller = nullptr;
    User* user = nullptr;

    QVBoxLayout vboxMain;
    QHBoxLayout hbox1;
    QHBoxLayout hbox2;
    QVBoxLayout vbox1; //change name
public:
    MainScreen(User* registeredUser); //invoke when registration has been successfully performed
    MainScreen(string username); //invoke when login is successfully performed
    ~MainScreen();

    void show(); //show the mainscreen
    void close(); //set the visiblity to false
};

#endif // MAINSCREEN_H
