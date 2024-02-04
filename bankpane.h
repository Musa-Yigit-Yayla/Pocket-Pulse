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
#include "bankingcontroller.h"
#include <cstddef>
#include <vector>
#include <unordered_map>

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
    vector<int> accountIndexes;

    unordered_map<QHBoxLayout*, QPushButton*> inspectMap;
    unordered_map<QHBoxLayout*, QPushButton*> closeMap;

public:
    explicit BankPane(User* user, QWidget *parent = nullptr);
    int getCurrentUserId() const;

    static const int GRID_HOR_SPACING = 20;
    static const int MID_POINT_SIZE = 16;
    static const int LARGE_POINT_SIZE = 20;
    static const QColor GOLDEN_COLOR;
    static const int TOOL_INSPECT_LENGTH = 25;

private:
    void setFormPane();
    void setLayoutManagement();
    QHBoxLayout* getAccountsRowBox(int id, BankingController& bc);

signals:

public slots:
    void slotGetAccount();
    void viewTransactions();
    void closeTransactions();
};
#endif // BANKPANE_H
