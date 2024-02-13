#include "abstractpane.h"
#include "user.h"
#include <QComboBox>
#include <QLineEdit>
#include <QWidget>
#include <QLabel>
#include <QGridLayout>
#include <QPushButton>
#include <QString>
#include <QTimer>
#include <cstddef>
#include <vector>
#include <unordered_map>

using namespace std;
#ifndef EXPENSEPANE_H
#define EXPENSEPANE_H


class ExpensePane: public AbstractPane{
Q_OBJECT
private:
    vector<QLineEdit*> textfields;
    vector<QLabel*> categoryLabels;
    QComboBox* dateSelector = nullptr;
    QPushButton* btUpdate = nullptr;
    QLabel* headerLabel = nullptr;
    QGridLayout* gridLayout = nullptr;
    QLabel* confirmationLabel = nullptr;

    static const vector<string> CATEGORY_LABEL_STRINGS;
    static unordered_map<string, int> monthMap;

public:
    ExpensePane(User* user, QWidget* parent = nullptr);
    static inline string getMonthString(int month);
    static inline int getMonthInteger(string month);

    static const string MONTHLY_GOALS_TABLENAME;
    enum class EXPENSE_CATEGORIES{HEALTH, EDUCATION, GROCERY_MARKET, ENTERTAINMENT, VEHICLE, FEES, OTHER, count}; //count stores the length of enum class
private:
    void setLayoutManagement();
    void setCombobox();

    const QString confirmSuccessStr = "Confirmed successfully";
    const QString confirmFailStr = "Couldn't update, try again";
public slots:
    void btUpdateSlot();
    void confirmationLabelSlot(bool confirmed);
    void confirmTimerSlot();
signals:
    void displayConfirmationLabel(bool confirmed);
};

#endif // EXPENSEPANE_H
