#include "bankpane.h"

BankPane::BankPane(QWidget *parent)
    : QWidget{parent}
{
    this->setFixedSize(MainScreen::GENERIC_PANE_WIDTH, MainScreen::GENERIC_PANE_HEIGHT);
}
