#ifndef BANKPANE_H
#define BANKPANE_H

#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include "abstractpane.h"

using namespace std;

class BankPane : public AbstractPane{
    Q_OBJECT

private:
    QGridLayout formPane;
    QLineEdit tfAccountID;
    QLineEdit tfBankID;
    QLabel accountLabel;
    QLabel bankLabel;
    QLabel errorLabel;
    QPushButton btGetAccount;

public:
    explicit BankPane(QWidget *parent = nullptr);

private:
    void setFormPane();

signals:

public slots:
    void slotGetAccount();
};

#endif // BANKPANE_H
