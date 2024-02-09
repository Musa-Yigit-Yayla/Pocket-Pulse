#include "abstractpane.h"
#include "user.h"
#include <QComboBox>
#include <QLineEdit>
#include <QWidget>
#include <QLabel>
#include <QGridLayout>
#include <QPushButton>
#include <QString>
#include <cstddef>
#include <vector>

using namespace std;
#ifndef EXPENSEPANE_H
#define EXPENSEPANE_H


class ExpensePane: public AbstractPane{
private:
    vector<QLineEdit*> textfields;
    vector<QLabel*> categoryLabels;
    QComboBox* dateSelector = nullptr;
    QPushButton* btUpdate = nullptr;
    QLabel* headerLabel = nullptr;
    QGridLayout* gridLayout = nullptr;

    static const vector<string> CATEGORY_LABEL_STRINGS;

public:
    ExpensePane(User* user, QWidget* parent = nullptr);

    enum class EXPENSE_CATEGORIES{HEALTH, EDUCATION, GROCERY_MARKET, ENTERTAINMENT, VEHICLE, FEES, OTHER, count}; //count stores the length of enum class
private:
    void setLayoutManagement();
public slots:
    void btUpdateSlot();
};

#endif // EXPENSEPANE_H
