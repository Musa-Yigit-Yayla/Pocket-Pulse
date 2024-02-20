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
#include <QStringList>
#include <QLabel>
#include <QObject>
#include <QDateEdit>
#include <QCheckBox>

/**
 * This pane displays user's past transactions in which he received money
 * The transactions will be sorted wrt date in ascending or descending order, or wrt to amount in ascending or descending order
 * Also the user will be able to provide start and begin dates (inclusive) to filter income retrieved at a certain time period
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
    QDateEdit* dateEditFrom = nullptr;
    QDateEdit* dateEditTo = nullptr; //inclusive
    QCheckBox* cbIgnoreDate = nullptr;

public:
    IncomePane(User* user, QWidget* parent = nullptr);

    const QStringList sortChoices = {"Most recent", "Oldest" , "Max amount", "Min amount"};
private:
    void displayIncome();
    vector<vector<int>> getSortedIncome();
public slots:
    void refreshHandler();
};

#endif // INCOMEPANE_H
