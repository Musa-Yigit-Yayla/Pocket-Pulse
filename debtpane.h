#include "abstractpane.h"
#include <user.h>
#include <cstddef>
#include <QWidget>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QPushButton>


#ifndef DEBTPANE_H
#define DEBTPANE_H

class DebtPane: public AbstractPane{
private:
    QGridLayout* gridPane = new QGridLayout(this);

    QPushButton* btAddDebt = new QPushButton(this);
public:
    DebtPane(User* user, QWidget* parent = nullptr);
public slots:
    void addDebtSlot();
};

#endif // DEBTPANE_H
