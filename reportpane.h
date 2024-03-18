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

    QLayout* expenseChartsPane = nullptr;

    QLayout* incomeExpenseDebtPane = nullptr;


    QGridLayout* monthPieChartPane = nullptr;
    QComboBox* comboBox = new QComboBox(this);

    QHBoxLayout* menuBox = new QHBoxLayout(this); // will enable the user to choose between panes
    PieChart* expenseDistribution = nullptr;

    QToolButton* tbECP = new QToolButton(this);
    QToolButton* tbIEDP = new QToolButton(this);
    QToolButton* tbMPCP = new QToolButton(this);

    QString toolButtonStrings[3] = {"Expense Charts", "Income Expense\n Debt Graphs", "Monthly Spenditures\n   Charts"};

public:
    ReportPane(User* user, QWidget* parent = nullptr);

private:
    void initExpenseChartsPane();
    void initIncomeExpenseDebtPane();
    void initMonthPieChartPane();
public slots:
    void menuSelectionSlot();
    void pieDateSelectionSlot(int index); //for the combobox

};

#endif // REPORTPANE_H
