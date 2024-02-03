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

    QHBoxLayout* pane; //high level container
    QScrollArea sa;
    QVBoxLayout* accountsBox = nullptr;
    QGridLayout* descriptionGrid = nullptr;
    QLabel totalSumLabel;

public:
    explicit BankPane(User* user, QWidget *parent = nullptr);
    int getCurrentUserId() const;

    static const int GRID_HOR_SPACING = 20;
    static const int MID_POINT_SIZE = 16;
    static const int LARGE_POINT_SIZE = 20;
    static const QColor GOLDEN_COLOR;

private:
    void setFormPane();
    void setLayoutManagement();

signals:

public slots:
    void slotGetAccount();
    void viewTransactions();
};
#endif // BANKPANE_H
