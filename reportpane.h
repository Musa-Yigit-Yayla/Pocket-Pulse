#include "abstractpane.h"
#include "piechart.h"
#include "rectwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QToolButton>
#include <QGridLayout>
#include <QComboBox>
#include <QCheckBox>
#include <QColor>
#include <QSpacerItem>

#ifndef REPORTPANE_H
#define REPORTPANE_H


class ReportPane: public AbstractPane{
private:
    QVBoxLayout* vbox = new QVBoxLayout(this); //vbox containing the menuBar and selected pane
    QHBoxLayout* menuBox = new QHBoxLayout(this); // will enable the user to choose between panes
    QToolButton* tbECP = new QToolButton(this);
    QToolButton* tbIEDP = new QToolButton(this);
    QToolButton* tbMPCP = new QToolButton(this);

    QWidget* chartWrapper = new QWidget();

    QGridLayout* goalsChartPane = nullptr;
    QComboBox* cbGoalDate = nullptr;
    PieChart* goalDistributionChart = nullptr;

    QGridLayout* incomeExpenseDebtPane = nullptr;
    QVBoxLayout* controlWrapper = nullptr;
    QCheckBox* dateAllCheckBox = nullptr;
    QPushButton* btRefresh = nullptr;
    QComboBox* fromComboBox = nullptr;
    QComboBox* toComboBox = nullptr;
    QScrollArea* rectSA = nullptr;
    QWidget* intermediateRectWrapper = nullptr;
    QGridLayout* rectGrid = nullptr;


    QGridLayout* monthPieChartPane = nullptr;
    QComboBox* comboBox = nullptr;
    PieChart* expenseDistributionChart = nullptr;



    QString toolButtonStrings[3] = {"Monthly Spenditure\nGoal Distribution", "Income Expense\n Debt Graphs", "Monthly Spenditure\n   Distribution"};
    static const QColor SENT_TRANSACTION_COLOR;
    static const QColor RECEIVED_TRANSACTION_COLOR;
    static const QColor TOTAL_SPENDITURE_GOAL_COLOR;

public:
    ReportPane(User* user, QWidget* parent = nullptr);
    static void deallocateItem(QLayoutItem* item); //static recursive method to deallocate a given layout item
private:
    void initGoalsChartPane();
    void initIncomeExpenseDebtPane();
    void initMonthPieChartPane();
protected:
    QWidget* getMonthBarChart(int month, int year);
    void deleteMonthBarChart(QWidget* barChart);
public slots:
    void menuSelectionSlot();
    void pieDateSelectionSlot(int index); //for the combobox
    void barChartRedrawSlot(int index); //for the date comboboxes of iedPane and its pushbutton

};

#endif // REPORTPANE_H
