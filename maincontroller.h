#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include "user.h"
#include <string>
#include <cstddef>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlError>

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

    static const string USER_TABLE_NAME;
};

#endif // MAINCONTROLLER_H
