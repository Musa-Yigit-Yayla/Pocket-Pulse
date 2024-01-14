#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include "user.h"
#include <string>
#include <cstddef>
#include <QSqlQuery>
#include <QSqlDatabase>

using namespace std;
/**
 * Controller class for interacting with the local database
*/
class MainController{
private:
    QSqlDatabase db;

public:
    MainController();
    ~MainController();
    static bool createUser(User* user);

    //static constants
    static const string DB_NAME;; //change the name when the app is deployed and ensure it matches the created db file
    static const string DB_PASSWORD;
};

#endif // MAINCONTROLLER_H
