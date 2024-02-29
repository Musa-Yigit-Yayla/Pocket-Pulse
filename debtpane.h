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

    //popup pane attributes
    QWidget* popupDebt = nullptr;
    QGridLayout* layout = nullptr;
    QLabel* expLabel = nullptr;
    QCheckBox* cbContactEnabled = nullptr;
    QLabel* labelOwedName = nullptr;
    QLabel* labelAmount = nullptr;
    QLabel* labelExp = nullptr;
    QLabel* labelDate = nullptr;
    QLineEdit* tfOwedName = nullptr;
    QLineEdit* tfAmount = nullptr;
    QLineEdit* tfExplanation = nullptr;
    QComboBox* cbContactName = nullptr;
    QLabel* errLabel = nullptr;
    QDateEdit* dateEditDue = nullptr;
    QPushButton* btReg = nullptr;

public:
    DebtPane(User* user, QWidget* parent = nullptr);
private:
    void initializeDebtPane();
public slots:
    void addDebtSlot();
    void btRegSlot();
    void contactCheckSlot(bool checked);




    //Draggable widget which wraps a QHBoxLayout containing information and controls regarding debts
    class DraggableDebt: public QWidget{
    private:
        int debtId;
        int debtPriority;
        QHBoxLayout* hbox = new QHBoxLayout(this);
        QPushButton* btMarkPaid = new QPushButton("Mark as paid", this);
        QLabel* labelName = nullptr;
        QLabel* labelAmount = nullptr;
        QLabel* labelExp = nullptr;
        QLabel* labelDate = nullptr;

    public:
        DraggableDebt(int debtId, int debtPriority, QWidget* parent = nullptr);
        int getPriority() const;
        void setPriority(int debtPriority);

    protected:
        //override and implement some of the mouse related event handling
        void mousePressEvent(QMouseEvent* event) override;
        void mouseMoveEvent(QMouseEvent* event) override;
        void mouseReleaseEvent(QMouseEvent* event) override;

    };
};

#endif // DEBTPANE_H
