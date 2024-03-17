#include "abstractpane.h"
#include "piechart.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QToolButton>

#ifndef REPORTPANE_H
#define REPORTPANE_H


class ReportPane: public AbstractPane{
private:
    QVBoxLayout* vbox = new QVBoxLayout(this); //vbox containing the menuBar and selected pane
    QLayout* pane1 = nullptr;
    QLayout* pane2 = nullptr;
    QLayout* pane3 = nullptr;
    QHBoxLayout* menuBox = new QHBoxLayout(this); // will enable the user to choose between panes
    PieChart* expenseDistribution = nullptr;


public:
    ReportPane(User* user, QWidget* parent = nullptr);
};

#endif // REPORTPANE_H
