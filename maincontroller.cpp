#include "maincontroller.h"
#include "user.h"
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
    return names;
}
const string MainController::DB_NAME = "PocketPulseDB";
const string MainController::DB_USERNAME = "root";
const string MainController::DB_PASSWORD = "123456";

const string MainController::USER_TABLE_NAME = "User";
