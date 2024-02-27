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
#include <QMouseEvent>
#include <unordered_map>


#ifndef DEBTPANE_H
#define DEBTPANE_H

class DebtPane: public AbstractPane{
private:
    QGridLayout* gridPane = new QGridLayout(this);
    QVBoxLayout* vbox = new QVBoxLayout(this);
    QPushButton* btAddDebt = new QPushButton(this);
public:
    DebtPane(User* user, QWidget* parent = nullptr);
public slots:
    void addDebtSlot();

    //Draggable widget which wraps a QHBoxLayout containing information and controls regarding debts
    class DraggableDebt: public QWidget{
    private:
        int debtId;
        QHBoxLayout* hbox = new QHBoxLayout(this);
        QPushButton* btMarkPaid = new QPushButton("Mark as paid", this);

    public:
        DraggableDebt(int debtId, QWidget* parent = nullptr);


    protected:
        //override and implement some of the mouse related event handling
        void mousePressEvent(QMouseEvent* event) override;
        void mouseMoveEvent(QMouseEvent* event) override;
        void mouseReleaseEvent(QMouseEvent* event) override;

    };
};

#endif // DEBTPANE_H
