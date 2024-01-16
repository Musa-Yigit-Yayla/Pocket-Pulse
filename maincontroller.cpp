#include "maincontroller.h"
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
    }
}
//Given user has to be checked from the caller to ensure that no such user exists
//Namely given user is expected to be a new user, not an existing one
bool MainController::createUser(User* user){
    bool success = false;
    if(user != nullptr){
        //check whether the User table exists or not
        if(!this->tableExists(MainController::USER_TABLE_NAME)){
            //create the user table
            QString createUserTable = "CREATE TABLE :userTableName (id INTEGER PRIMARY KEY, name TEXT, password TEXT, savecode TEXT);";
            QSqlQuery userTableQuery(this->db);
            userTableQuery.prepare(createUserTable);
            userTableQuery.bindValue(":userTableName", QString::fromStdString(USER_TABLE_NAME));
            userTableQuery.exec();
        }

        //proceed registering the user
        QString insertUser = "INSERT INTO :userTableName VALUES (NULL, ':userName', ':password', ':savecode');";
        QSqlQuery query(this->db);
        query.prepare(insertUser);
        query.bindValue(":userTableName", QString::fromStdString(USER_TABLE_NAME));
        query.bindValue(":userName", QString::fromStdString(user->getUserName()));
        query.bindValue(":password", QString::fromStdString(user->getPassword()));
        query.bindValue(":savecode", QString::fromStdString(user->getSaveCode()));

        success = query.exec();
    }
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
const string MainController::DB_NAME = "PocketPulseDB";
const string MainController::DB_USERNAME = "root";
const string MainController::DB_PASSWORD = "123456";

const string MainController::USER_TABLE_NAME = "User";
