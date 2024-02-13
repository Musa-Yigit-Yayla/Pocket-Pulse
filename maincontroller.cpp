#include "maincontroller.h"
#include "user.h"
#include "expensepane.h"
#include <iostream>

using namespace std;
MainController::MainController(){
    //Create and establish the database connection
    this->db = QSqlDatabase::addDatabase("QSQLITE");
    this->db.setHostName("localhost");
    this->db.setPort(3306);
    this->db.setPassword(QString::fromStdString(*(const_cast<string*>(&DB_PASSWORD))));
    this->db.setDatabaseName(QString::fromStdString(*(const_cast<string*>(&DB_NAME))));
    this->db.setUserName(QString::fromStdString(*(const_cast<string*>(&DB_USERNAME))));

    //attempt to open the database
    if(this->db.open()){
        cout << "Debug: Database connection has been established successfuly in MainController constructor" << endl;
    }

}
MainController::~MainController(){
    if(this->db.isOpen()){
        this->db.close();
        if(!this->db.isOpen()){
            cout << "Debug: Database connection has been closed successfully" << endl;
        }

    }
}
//Given user has to be checked from the caller to ensure that no such user exists
//Namely given user is expected to be a new user, not an existing one
bool MainController::createUser(User* user){
    bool success = false;
    cout << "Debug: MainController::createUser has been invoked" << endl;
    if(user != nullptr){
        //check whether the User table exists or not
        if(!this->tableExists(MainController::USER_TABLE_NAME)){
            cout << "Debug: createUser execution entered table creation condition" << endl;
            //create the user table
            QString createUserTable = QString::fromStdString("CREATE TABLE " + USER_TABLE_NAME +  " (id INTEGER PRIMARY KEY, name TEXT, password TEXT, savecode TEXT, imageurl TEXT);");
            QSqlQuery userTableQuery(this->db);
            userTableQuery.prepare(createUserTable);
            //userTableQuery.bindValue(":userTableName", QString::fromStdString(USER_TABLE_NAME));
            userTableQuery.exec();
        }

        //proceed registering the user
        QString insertUser = QString::fromStdString("INSERT INTO " + USER_TABLE_NAME + " VALUES (NULL, :userName, :password, :savecode, :imgurl);");
        QSqlQuery query(this->db);
        query.prepare(insertUser);
        //query.bindValue(":userTableName", QString::fromStdString(USER_TABLE_NAME));
        query.bindValue(":userName", QString::fromStdString(user->getUserName()));
        query.bindValue(":password", QString::fromStdString(user->getPassword()));
        query.bindValue(":savecode", QString::fromStdString(user->getSaveCode()));
        query.bindValue(":imgurl", QString::fromStdString(user->getProfileImagePath()));

        success = query.exec();
        if(!success){
            qDebug() << "Debug: Error inserting the user: " << (query.lastError());
        }
    }
    cout << "Debug: MainController::createUser is returning success " << success << endl;
    return success;
}
bool MainController::userExists(string username){
    bool success = false;

    QSqlQuery query(this->db);
    query.prepare(QString::fromStdString("SELECT * FROM " + USER_TABLE_NAME + " WHERE name = :username;"));
    query.bindValue(QString::fromStdString(":username"), QString::fromStdString(username));

    success = query.exec();
    cout << "Debug: userExists query.exec is " << success << endl;

    if(!success){
        qDebug() << "Debug: userExists query error is: " << query.lastError().text();
    }

    success = success & query.next();
    cout << "Debug: userExists query.next is " << success << endl;
    cout << "Debug: returning " << success << " from MainController::userExists" << endl;
    return success;
}
bool MainController::tableExists(string tableName){
    QSqlQuery sq(this->db);
    sq.prepare("SELECT * FROM sqlite_master WHERE type='table' AND name=:tableName");
    sq.bindValue(":tableName", QString::fromStdString(tableName));

    if(!sq.exec()){
        cout << "Debug: query in tableExists cannot be executed" << endl;
    }
    return sq.next();

}
string MainController::getPassword(string username){
    string result = "";
    QSqlQuery sq(this->db);
    sq.prepare(QString::fromStdString("SELECT password FROM " + USER_TABLE_NAME + " WHERE name = :username;"));
    sq.bindValue(":username", QString::fromStdString(username));
    if(sq.exec() && sq.next()){
        //password retrieved successfully
        result = (sq.value(0).toString()).toStdString();
    }
    return result;
}
string MainController::getSavecode(string username){
    string result = "";
    QSqlQuery sq(this->db);
    sq.prepare(QString::fromStdString("SELECT savecode FROM " + USER_TABLE_NAME + " WHERE name = :username;"));
    sq.bindValue(":username", QString::fromStdString(username));
    if(sq.exec() && sq.next()){
        result = (sq.value(0).toString()).toStdString();
    }
    return result;
}
//Method to retrieve each and every username existing on the local db
vector<string> MainController::getUsernames(){
    QSqlQuery* sq = new QSqlQuery(this->db);
    sq->prepare(QString::fromStdString("SELECT name FROM " + USER_TABLE_NAME + ";"));
    vector<string> names;

    if(sq->exec()){
        while(sq->next()){
            names.push_back(sq->value(0).toString().toStdString());
        }
    }
    delete sq;
    return names;
}
int MainController::getUserCount(){
    QSqlQuery* sq = new QSqlQuery(this->db);
    sq->prepare(QString::fromStdString("SELECT * FROM " + USER_TABLE_NAME + ";"));
    int count = 0;
    if(sq->exec() && sq->next()){ //the first next call reaches to the 0th index if execution is successfull
        while(sq->next()){
            count++;
        }
    }
    delete sq;
    return count;
}
bool MainController::updatePassword(string username, string password){
    QSqlQuery query(this->db);
    query.prepare(QString::fromStdString("UPDATE " + USER_TABLE_NAME + " SET password = :passvalue WHERE name = :uservalue;"));
    query.bindValue(":passvalue", QString::fromStdString(password));
    query.bindValue(":uservalue", QString::fromStdString(username));

    return query.exec();
}
int MainController::getUserId(string username){
    int result = -1;
    QSqlQuery sq(this->db);
    sq.prepare(QString::fromStdString("SELECT id FROM " + USER_TABLE_NAME + " WHERE name = :username;"));
    sq.bindValue(":username", QString::fromStdString(username));

    if(sq.exec() && sq.next()){
        result = sq.value(0).toInt();
    }
    return result;
}
void MainController::createMonthlyExpenseGoalsTable(){
    if(!this->tableExists(ExpensePane::MONTHLY_GOALS_TABLENAME)){
        //create the corresponding table
        QSqlQuery sq(this->db);
        sq.prepare(QString::fromStdString("CREATE TABLE " + ExpensePane::MONTHLY_GOALS_TABLENAME + " (id INTEGER PRIMARY KEY AUTOINCREMENT, user_name TEXT, month INTEGER, year INTEGER, health INTEGER, education INTEGER, market_grocery INTEGER, entertainment INTEGER, vehicle INTEGER, fees INTEGER, other INTEGER);"));
        sq.exec();
    }

}
//Returns a bool value indicating whether a match in the monthly expense goals table has found with the given userid
bool MainController::monthlyExpenseGoalsExist(string userName){
    QSqlQuery sq(this->db);
    sq.prepare(QString::fromStdString("SELECT * FROM " + ExpensePane::MONTHLY_GOALS_TABLENAME + " WHERE user_name = :userName;"));
    sq.bindValue(":userName", QString::fromStdString(userName));

    bool success = sq.exec();
    bool found = success && sq.next();

    return found;
}
//values vector will expectedly have monthly goal values specified by the user, sentinel -1 denotes that no goal has been specified for that category
bool MainController::registerUserMonthlyGoals(string username, int month, int year, vector<int>& values){
    //if a tuple with the primary keys username, month, and year do not exist, create one
    QSqlQuery sq(this->db);
    sq.prepare(QString::fromStdString("SELECT * FROM " + ExpensePane::MONTHLY_GOALS_TABLENAME + " WHERE (user_name = :userName AND month = :month AND year = :year);"));
    sq.bindValue(":userName", QString::fromStdString(username));
    sq.bindValue(":month", month);
    sq.bindValue(":year", year);

    bool entryFound = sq.exec() && sq.next();
    bool success = false;
    if(entryFound){
        //update the entry
        string updateSubstr = "";
        for(int i = 0; i < values.size(); i++){
            if(values.at(i) != -1){
                string currStr = monthly_goal_categories_columns.at(i) + " = :value" + to_string(i);
                if(i != values.size() - 1){
                    currStr += ", ";
                }
                updateSubstr += currStr;
            }
        }
        if(updateSubstr != ""){
            sq.prepare(QString::fromStdString("UPDATE " + ExpensePane::MONTHLY_GOALS_TABLENAME + " SET " + updateSubstr + " WHERE user_name = :userName AND month = :month AND year = :year;"));
            sq.bindValue(":userName", QString::fromStdString(username));
            sq.bindValue(":month", month);
            sq.bindValue(":year", year);

            for(int i = 0; i < values.size(); i++){
                sq.bindValue(QString::fromStdString(":value" + to_string(i)), values.at(i));
            }

            success = sq.exec();
            qDebug() << "Debug: registerUserMonthlyGoals UPDATE query has been executed with success " << success;
        }
    }
    else{
        string columnsSubstr = "";
        string valuesSubstr = "";
        for(int i = 0; i < values.size(); i++){
            if(values.at(i) != -1){
                columnsSubstr += monthly_goal_categories_columns.at(i);
                valuesSubstr += ":value" + to_string(i);

                if(i != values.size() - 1){
                    columnsSubstr += ", ";
                    valuesSubstr += ", ";
                }
            }
        }
        if(columnsSubstr != "" && valuesSubstr != ""){
            sq.prepare(QString::fromStdString("INSERT INTO " + ExpensePane::MONTHLY_GOALS_TABLENAME + " (user_name, month, year, " + columnsSubstr + ") VALUES (:username, :month, :year, " + valuesSubstr + ");"));
            sq.bindValue(":username", QString::fromStdString(username));
            sq.bindValue(":month", month);
            sq.bindValue(":year", year);

            for(int i = 0; i < values.size(); i++){
                QString placeholder = QString::fromStdString(":value" + to_string(i));
                sq.bindValue(placeholder, values.at(i));
            }

            success = sq.exec();
            qDebug() << "Debug: registerUserMonthlyGoals INSERT INTO query has been executed with success " << success;
            qDebug() << "Debug: lastError in the query is: " << sq.lastError();
        }
    }
    return success;
}
const string MainController::DB_NAME = "PocketPulseDB";
const string MainController::DB_USERNAME = "root";
const string MainController::DB_PASSWORD = "123456";

const string MainController::USER_TABLE_NAME = "User";
const vector<string> MainController::monthly_goal_categories_columns = {"health", "education", "market_grocery", "entertainment", "vehicle", "fees", "other"};
