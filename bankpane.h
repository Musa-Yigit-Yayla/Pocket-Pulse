#ifndef BANKPANE_H
#define BANKPANE_H

#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include "abstractpane.h"
#include <cstddef>

using namespace std;

class BankPane : public AbstractPane{
    Q_OBJECT

private:
    QGridLayout formPane;
    QLineEdit tfAccountID;
    QLineEdit tfAccPass;
    QLabel accountLabel;
    QLabel passLabel;
    QLabel errorLabel;
    QPushButton btGetAccount;

    QScrollArea sa;
    QVBoxLayout* accountBox = nullptr;

public:
    explicit BankPane(QWidget *parent = nullptr);

private:
    void setFormPane();
    void setLayoutManagement();

signals:

public slots:
    void slotGetAccount();
    void viewTransactions();
};

#endif // BANKPANE_H
