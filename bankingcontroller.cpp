#include "bankingcontroller.h"
#include "qsqlerror.h"

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
    qDebug() << "Debug: Database connection open status in BankingController::registerAccountToUser is " << this->db.isOpen();
    if(!this->accountRegistered(accountId)){
        QSqlQuery sq(this->db);
        sq.prepare(QString::fromStdString("INSERT INTO " + this->USER_ACCOUNT_TABLE_NAME + " (account_id, user_id) VALUES (:accountId, :userId);"));
        sq.bindValue(":accountId", accountId);
        sq.bindValue(":userId", userId);
        success = sq.exec();

        if(!success){
            //display errors
            QSqlError error = sq.lastError();
            qDebug() << "Debug: error is: " << error.text();
        }
    }
    return success;
}
/**
 * @brief BankingController::getPastTransactions retrieves the transactionhistory
 * @param accountId
 * @return the past transactions the given account was involved as a 2d string vector with sorted in descending order wrt date
 * each row contains data about a transaction excluding id (transaction id) and conserving the order of table columns
 */
vector<vector<string>> BankingController::getPastTransactions(int accountId){
    vector<vector<string>> result;

    QSqlQuery sq(this->db);
    sq.prepare(
        QString::fromStdString("SELECT sender_id, receiver_id, amount, category, date FROM " + this->TRANSACTION_TABLE_NAME + " WHERE (sender_id = :accountId OR receiver_id = :accountId);")
        );
    sq.bindValue(":accountId", accountId);

    if(sq.exec() && sq.next()){
        //query executed successfully and there is at least one record
        int relatedColumnCount = 6;
        do{
            vector<string> currRow;
            for(int i = 1; i <= relatedColumnCount; i++){
                currRow.push_back(sq.value(i).toString().toStdString());
            }
            result.push_back(currRow);
        }while(sq.next());
    }
    return result;
}
