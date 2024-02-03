#include "bankingcontroller.h"

using namespace std;

BankingController::BankingController(){
    this->db = QSqlDatabase::addDatabase("QSQLITE", "sqlite_banking_connection");
    this->db.setDatabaseName(QString::fromStdString(this->DB_NAME));
    this->db.setHostName("localhost");
    this->db.setPort(3307);
    this->db.setHostName(QString::fromStdString(this->HOST_NAME));
    this->db.setPassword(QString::fromStdString(this->DB_PASS));

    this->db.open();
    qDebug() << "Debug: BankingController database connection establishment attempt has yielded " << this->db.isOpen();

}
bool BankingController::accountExists(int accountId){
    QSqlQuery sq(this->db);
    sq.prepare(QString::fromStdString("SELECT * FROM " + this->ACCOUNT_TABLE_NAME + " WHERE account_id = :givenId;"));
    sq.bindValue(":givenId", accountId);

    bool success = sq.exec();
    success = success && sq.next();
    qDebug() << "Debug: account with id " << accountId << " found state is " << success;
    return success;
}
//If the specified account attribute is id, an empty string is returned
//Otherwise, an attempt to retrieve the corresponding field is made and result is returned
string BankingController::getAccountAttribute(int accountId, BankingController::ACCOUNT_ATTRIBUTES attribute){
    string columnName;
    string result = "";

    switch(attribute){
        case BankingController::ACCOUNT_ATTRIBUTES::ID: return result; break;
        case BankingController::ACCOUNT_ATTRIBUTES::FIRST_NAME: columnName = "first_name"; break;
        case BankingController::ACCOUNT_ATTRIBUTES::LAST_NAME: columnName = "last_name"; break;
        case BankingController::ACCOUNT_ATTRIBUTES::EMAIL: columnName = "email"; break;
        case BankingController::ACCOUNT_ATTRIBUTES::BANK_PASSWORD: columnName = "bank_password"; break;
        case BankingController::ACCOUNT_ATTRIBUTES::BALANCE: columnName = "balance"; break;
    }
    QSqlQuery sq(this->db);
    sq.prepare(QString::fromStdString("SELECT " + columnName + " FROM " + this->ACCOUNT_TABLE_NAME + " WHERE account_id = :givenId;"));
    sq.bindValue(":givenId", accountId);

    bool executed = sq.exec();
    bool iteratedNext = sq.next();
    if(executed && iteratedNext){
        result = sq.value(0).toString().toStdString();
    }
    qDebug() << "Debug: returned value from getAccountAttribute is " << result << " and executed and iteratedNext are respectively " << executed << " and " << iteratedNext;
    return result;
}
//Returns the accounts of the user (if any) with respect to relational table
vector<int> BankingController::getAccountsOfUser(int userId){
    QSqlQuery sq(this->db);
    vector<int> result;

    sq.prepare(QString::fromStdString("SELECT account_id FROM " + this->USER_ACCOUNT_TABLE_NAME + " WHERE user_id = :userId;"));
    sq.bindValue(":userId", userId);

    if(sq.exec()){
        //iterate over yielded query values and push them into the result vector
        int counter = 0;
        while(sq.next()){
            string currStr = ((sq.value(0)).toString()).toStdString();
            result.push_back(stoi(currStr));
        }
    }
    return result;
}
//Checks whether a given account is registered to any user
bool BankingController::accountRegistered(int accountId){
    QSqlQuery sq(this->db);
    bool found = false;

    sq.prepare(QString::fromStdString("SELECT user_id FROM " + this->USER_ACCOUNT_TABLE_NAME + " WHERE account_id = :accountId;"));
    sq.bindValue(":accountId", accountId);

    found = sq.exec();
    found = found & sq.next();
    return found;
}
bool BankingController::registerAccountToUser(int accountId, int userId){
    bool success = false;
    if(!this->accountRegistered(accountId)){
        QSqlQuery sq(this->db);
        sq.prepare(QString::fromStdString("INSERT INTO " + this->USER_ACCOUNT_TABLE_NAME + " (account_id, user_id) VALUES (:accountId, :userId);"));
        sq.bindValue(":accountId", accountId);
        sq.bindValue(":userId", userId);
        success = sq.exec();
    }
    return success;
}
