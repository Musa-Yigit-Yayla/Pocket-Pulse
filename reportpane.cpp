#include "reportpane.h"


ReportPane::ReportPane(User* user, QWidget* parent): AbstractPane{user, parent}{
    //initialize the menubox by placing toolbuttons into it

    tbECP->setText(this->toolButtonStrings[0]);
    tbIEDP->setText(this->toolButtonStrings[1]);
    tbMPCP->setText(this->toolButtonStrings[2]);

    //initially tbECP option is selected, so mark its stroke to green in CSS
    tbECP->setStyleSheet("stroke-color: green;");

    this->menuBox->addWidget(tbECP);
    this->menuBox->addWidget(tbIEDP);
    this->menuBox->addWidget(tbMPCP);
    this->menuBox->setSpacing(20);

    this->initExpenseChartsPane();

}
void ReportPane::initExpenseChartsPane(){

}
void ReportPane::initIncomeExpenseDebtPane(){

}
void ReportPane::initMonthPieChartPane(){
    this->monthPieChartPane = new QGridLayout(this);
    //set the combobox content to the months along with years in which user has made spending transactions using any of their registered accounts

}
void ReportPane::menuSelectionSlot(){

}
