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
    void registerUserMonthlyGoals(string username, int month, int year, vector<int> values);

    static const string USER_TABLE_NAME;
    static const vector<string> monthly_goal_categories_columns;
};

#endif // MAINCONTROLLER_H
