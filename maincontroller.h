#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

//#include "user.h"
#include <string>
#include <cstddef>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlError>
#include <qDebug>
#include <QVariant>
#include <QString>
#include <vector>
#include <QVariant>
#include <algorithm>
#include <unordered_map>

class User;
using namespace std;
/**
 * Controller class for interacting with the local database
*/
class MainController{
private:
    QSqlDatabase db;

    //static constants
    static const string DB_NAME; //change the name when the app is deployed and ensure it matches the created db file
    static const string DB_PASSWORD;
    static const string DB_USERNAME;


public:
    MainController();
    ~MainController();
    bool createUser(User* user);
    bool tableExists(string tableName);
    bool userExists(string username);
    string getPassword(string username);
    string getSavecode(string username);
    int getUserId(string username);
    bool updatePassword(string username, string password);
    vector<string> getUsernames();
    int getUserCount();
    bool monthlyExpenseGoalsExist(string username);
    void createMonthlyExpenseGoalsTable();
    bool registerUserMonthlyGoals(string username, int month, int year, vector<int>& values);
    vector<int> getUserMonthlyGoals(string username, int month, int year);
    vector<vector<int>> getMonthsWithExpenseGoals(string username);
    bool createUserContactsTable();
    vector<vector<QString>> retrieveContacts(string& username);
    bool addContact(string username, string contactName, string category, string explanation);
    bool userHasContact(string username, string contactName);
    bool updateContact(string username, string contactName, string newContactName, string category, string explanation);
    bool deleteContact(string username, string contactName);
    int registerDebt(string username, string owedName, int amount, string explanation, string due_date, int paid_status);
    vector<vector<QVariant>> getAllDebts(string username, bool selectCurrOnly);
    vector<QVariant> getDebt(int debtId);
    int getDebtPriority(int debtId);
    bool setDebtPriority(int debtId, int debtPriority);
    bool markDebtAsPaid(int debtId);
    bool registerFinancialGoal(string username, string explanation, string dateOfCreation);
    unordered_map<int, string> retrieveFinancialGoals(string username, bool reached);
    bool markFinancialGoalDone(int goalID);
    vector<int> getMaxExpenseGoalSpan(string username); //returns the minimum and maximum dates (month, year) in a 1d vector
    vector<int> getDebtStatusRatio(string username);
    vector<int> getFinancialGoalsStatusRatio(int userID);

    static const string USER_TABLE_NAME;
    static const string USER_CONTACTS_TABLE_NAME;
    static const string USER_DEBTS_TABLE_NAME;
    static const string FINANCIAL_GOALS_TABLE_NAME;
    static const vector<string> monthly_goal_categories_columns;
};

#endif // MAINCONTROLLER_H
