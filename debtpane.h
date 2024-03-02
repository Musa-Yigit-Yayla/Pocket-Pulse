#include "abstractpane.h"
#include <user.h>
#include <cstddef>
#include <QWidget>
#include <QGridLayout>
#include <QPoint>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>
#include <QDateEdit>
#include <QIntValidator>
#include <QToolButton>
#include <QSpacerItem>
#include <QMouseEvent>
#include <unordered_map>


#ifndef DEBTPANE_H
#define DEBTPANE_H

class DebtPane: public AbstractPane{
Q_OBJECT
private:
    QGridLayout* gridPane = new QGridLayout(this);
    QScrollArea* sa = new QScrollArea(this);
    QVBoxLayout* vbox = new QVBoxLayout(this);
    QPushButton* btAddDebt = new QPushButton("Regiser a debt", this);

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
    QVBoxLayout*& getVBox();
    void refreshVBox();
private:
    void initializeDebtPane();
    inline void setContactComboBox();
public slots:
    void addDebtSlot();
    void btRegSlot();
    void contactCheckSlot(bool checked);

};

//Draggable widget which wraps a QHBoxLayout containing information and controls regarding debts
class DraggableDebt: public QWidget{
    Q_OBJECT
private:
    int debtId;
    int debtPriority;
    QHBoxLayout* hbox = new QHBoxLayout(this);
    QToolButton* btMarkPaid = new QToolButton(this);
    QLabel* labelName = nullptr;
    QLabel* labelAmount = nullptr;
    QLabel* labelExp = nullptr;
    QLabel* labelDate = nullptr;
    QPoint initialPos;
    bool initialPosSet = false;



public:
    DraggableDebt(int debtId, int debtPriority, QString& owedName, QString& amount, QString& explanation, QString& date, QWidget* parent = nullptr);
    int getPriority() const;
    void setPriority(int debtPriority);
    void resetInitialPosSet();

    static const int MAX_WIDTH = 1000;
    static const int MAX_HEIGHT = 250;
    static const QPoint MOVE_OFFSET;

private:
    void setInitialPos(QPoint point);

protected:
    //override and implement some of the mouse related event handling
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

protected slots:
    void markAsPaidSlot();


};

#endif // DEBTPANE_H
