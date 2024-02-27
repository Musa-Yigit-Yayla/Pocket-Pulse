#include "abstractpane.h"
#include <user.h>
#include <cstddef>
#include <QWidget>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>
#include <QDateEdit>
#include <QIntValidator>
#include <unordered_map>


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

    //Draggable widget which wraps a QHBoxLayout containing information and controls regarding debts
    class DraggableDebt: public QWidget{
    private:
        QHBoxLayout* hbox = new QHBoxLayout(this);

    public:
        DraggableDebt(QWidget* parent = nullptr);
    };
};

#endif // DEBTPANE_H
