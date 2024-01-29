#include "bankpane.h"
#include "mainscreen.h"

using namespace std;

BankPane::BankPane(QWidget *parent)
    : QWidget{parent}
{
    this->setFixedSize(MainScreen::GENERIC_PANE_WIDTH, MainScreen::GENERIC_PANE_HEIGHT);
}
