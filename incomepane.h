#include "abstractpane.h"
#include "user.h"
#include <QWidget>
#include <cstddef>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QComboBox>
#include <QPushButton>
#include <QGridLayout>
#include <QString>
#include <QLabel>
#include <QObject>

/**
 * This pane displays user's past transactions in which he received money
 * The transactions will be sorted wrt date in ascending or descending order, or wrt to amount in ascending or descending order
 */
#ifndef INCOMEPANE_H
#define INCOMEPANE_H

class IncomePane: public AbstractPane{
Q_OBJECT

private:
    QGridLayout* grid = nullptr; //main pane
    QVBoxLayout* vbox = nullptr;
    QScrollArea* sa = nullptr;
    QPushButton* btRefresh = nullptr;
    QComboBox* cbSortSelector = nullptr;
public:
    IncomePane(User* user, QWidget* parent = nullptr);
private:
    void displayIncome();
public slots:
    void refreshHandler();
};

#endif // INCOMEPANE_H
