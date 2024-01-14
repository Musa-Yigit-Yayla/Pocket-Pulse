#include "maincontroller.h"

using namespace std;
MainController::MainController(){
    //Create and establish the database connection
    this->db = QSqlDatabase::addDatabase("QMYSQL");

}
MainController::~MainController(){

}
//Given user has to be checked from the caller to ensure that no such user exists
bool MainController::createUser(User* user){
    if(user != nullptr){
        //proceed registering the user
    }
}
const string MainController::DB_NAME = "PocketPulseDB";
const string MainController::DB_PASSWORD = "123456";
