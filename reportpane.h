#include "abstractpane.h"
#include "piechart.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QToolButton>
#include <QGridLayout>
#include <QComboBox>

#ifndef REPORTPANE_H
#define REPORTPANE_H


class ReportPane: public AbstractPane{
private:
    QVBoxLayout* vbox = new QVBoxLayout(this); //vbox containing the menuBar and selected pane
    QHBoxLayout* menuBox = new QHBoxLayout(this); // will enable the user to choose between panes
    QToolButton* tbECP = new QToolButton(this);
    QToolButton* tbIEDP = new QToolButton(this);
    QToolButton* tbMPCP = new QToolButton(this);

    QGridLayout* goalsChartPane = nullptr;
    QComboBox* cbGoalDate = new QComboBox();
    PieChart* goalDistributionChart = nullptr;

    QLayout* incomeExpenseDebtPane = nullptr;


    QGridLayout* monthPieChartPane = nullptr;
    QComboBox* comboBox = new QComboBox();
    PieChart* expenseDistributionChart = nullptr;



    QString toolButtonStrings[3] = {"Monthly Financial\nGoal Distribution", "Income Expense\n Debt Graphs", "Monthly Spenditure\n   Distribution"};

public:
    ReportPane(User* user, QWidget* parent = nullptr);

private:
    void initGoalsChartPane();
    void initIncomeExpenseDebtPane();
    void initMonthPieChartPane();
public slots:
    void menuSelectionSlot();
    void pieDateSelectionSlot(int index); //for the combobox

};

#endif // REPORTPANE_H
