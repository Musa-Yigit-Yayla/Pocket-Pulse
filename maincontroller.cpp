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
bool MainController::createUser(User* user){
    bool success = false;
    if(user != nullptr){
        //proceed registering the user
    }
    return success;
}
const string MainController::DB_NAME = "PocketPulseDB";
const string MainController::DB_USERNAME = "root";
const string MainController::DB_PASSWORD = "123456";
