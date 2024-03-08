#include "bankingcontroller.h"
#include "qsqlerror.h"
#include "maincontroller.h"
#include <QDate>

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
//Returns a string vector which has the first name at the 0th index, and last name at 1th index
//Returns empty vector if no match is obtained
vector<string> BankingController::getFullNameByAccId(int accountId){
    vector<string> result;
    QSqlQuery query(this->db);
    query.prepare(QString::fromStdString("SELECT first_name, last_name FROM " + this->ACCOUNT_TABLE_NAME + " WHERE account_id = :accountId;"));
    query.bindValue(":accountId", accountId);

    if(query.exec() && query.next()){
        result.push_back((query.value(0).toString()).toStdString());
        result.push_back(query.value(1).toString().toStdString());
    }
    return result;
}
//returns the past transactions in which the user has received money, as a 2d int vector containing {senderId, amount, date} for each row
//(date is in the form of days)
vector<vector<int>> BankingController::retrieveIncome(const string username){
    //First retrieve the data from transactios table and store it in a 2d vector
    //Each row will contain 3 columns, sender id, sent date in the form of days, and sent amount as an integer
    vector<vector<int>> result;

    //first retrieve the user id via using maincontroller
    MainController mc;
    int userId = mc.getUserId(username);

    if(userId != -1){ //user with the given username exists
        //now get the user's registered accounts
        vector<int> accounts = this->getAccountsOfUser(userId);
        for(int currAccId: accounts){
            //select each transaction here receiver id matches the currAccId, also apply correct projections
            QSqlQuery sq(this->db);
            sq.prepare(QString::fromStdString("SELECT sender_id, amount, date FROM " + TRANSACTION_TABLE_NAME + " WHERE receiver_id = :accountId"));
            sq.bindValue(":accountId", currAccId);
            if(sq.exec()){
                while(sq.next()){
                    vector<int> currRow;
                    currRow.push_back(sq.value(0).toInt());
                    string amount = sq.value(1).toString().toStdString().substr(1);
                    currRow.push_back(stoi(amount));

                    //retrieve the number of elapsed days since 1/1/2020
                    vector<int> currDate = splitDate(sq.value(2).toString().toStdString());
                    QDate transactionDate(currDate.at(2), currDate.at(0), currDate.at(1));
                    int elapsedDays = fromDate.daysTo(transactionDate);
                    currRow.push_back(elapsedDays);

                    result.push_back(currRow);
                }
            }
        }
    }
    return result;
}
//Category 0 specifies select all transactions
//Returned transactions are sorted wrt date of transaction
vector<vector<QVariant>> BankingController::getSpentTransactions(const int userId, const int month, const int year, int category = 0){
    vector<vector<QVariant>> result;
    QSqlQuery sq(this->db);

    string datePattern0 = to_string(month) + "/_/" + to_string(year);
    string datePattern1 = to_string(month) + "/__/" + to_string(year);

    //we should apply this process to all accounts of the given user
    vector<int> accounts = this->getAccountsOfUser(userId);

    for(int accountId: accounts){
        if(category == 0){
            sq.prepare(QString::fromStdString("SELECT * FROM " + TRANSACTION_TABLE_NAME + " WHERE sender_id = :accountId AND (date LIKE :pattern0 OR date LIKE :pattern1)"
                                                                                          " ORDER BY date DESC;"));
        }
        else{
            //select from the specified category
            sq.prepare(QString::fromStdString("SELECT * FROM " + TRANSACTION_TABLE_NAME + " WHERE sender_id = :accountId AND category = :category AND (date LIKE :pattern0 OR date LIKE :pattern1)"
                                                                                          " ORDER BY date DESC;"));
            sq.bindValue(":category", category);
        }

        sq.bindValue(":accountId", accountId);
        sq.bindValue(":pattern0", QString::fromStdString(datePattern0));
        sq.bindValue(":pattern1", QString::fromStdString(datePattern1));

        bool success = sq.exec();
        qDebug() << "Debug: BankingController::getSpentTransactions query execution yielded " << success;


        if(success && sq.next()){
            qDebug() << "Debug: about to execute the do while loop of getSpentTransactions";

            int columnCount = 6;
            //do while for fun :)
            do{

                vector<QVariant> currRow;
                //insert all data for possible usage of any of them
                for(int i = 0; i < columnCount; i++){
                    currRow.push_back(sq.value(i));
                }
                result.push_back(currRow);
            }while(sq.next());
        }
        else{
            qDebug() << "Debug: no viable transaction for account " << accountId << " in getSpentTransactions has been found.";
        }
    }




    return result;
}
//When month and year is not specified, all of the transactions with the given category regardless of transaction dates will be summed and returned
int BankingController::sumSentTransactions(int userId, int category, int month, int year){
    int result = 0;

    //Perform the following loop body for each and every account of the given user
    vector<int> accounts = this->getAccountsOfUser(userId);

    for(int currAccount: accounts){
        //use aggregate sum to retrieve the total sum of sent transactions by their category
        QSqlQuery sq(this->db);
        if(month == -1 && year == -1){
            sq.prepare(QString::fromStdString("SELECT amount FROM " + TRANSACTION_TABLE_NAME + " WHERE sender_id = :accountId AND category = :category;"));
        }
        else{
            QString datePattern = QString::fromStdString(to_string(month) + "/_%/" + to_string(year));
            sq.prepare(QString::fromStdString("SELECT amount FROM " + TRANSACTION_TABLE_NAME + " WHERE sender_id = :accountId AND category = :category "
                                                                                                              "AND date LIKE :datePattern;"));
            sq.bindValue(":month", month);
            sq.bindValue(":year", year);
            sq.bindValue(":datePattern", datePattern);
        }
        sq.bindValue(":accountId", currAccount);
        sq.bindValue(":category", category);

        if(sq.exec()){
            while(sq.next()){
                result += stod(sq.value(0).toString().toStdString().substr(1)); //auto cast to int
            }

        }
    }

    qDebug() << "Debug: returning from sumSentTransactions the value " << result << " for the given category " << category;
    return result;
}
//Receives a date string in the format m/d/y
//Returns a vector containing {month, day, year} as int
inline vector<int> BankingController::splitDate(const string givenDate){
    vector<int> result;

    int index1 = -1, index2 = -1;
    int counter = 0;
    while(counter < givenDate.size()){
        if(givenDate.at(counter) == '/'){
            if(index1 == -1){
                index1 = counter;
            }
            else{
                index2 = counter;
            }
        }
        counter++;
    }

    if(index1 != -1 && index2 != -1){
        result.push_back(stoi(givenDate.substr(0, index1)));
        result.push_back(stoi(givenDate.substr(index1 + 1, index2)));
        result.push_back(stoi(givenDate.substr(index2 + 1)));
    }
    return result;
}
const QDate BankingController::fromDate(2020, 1, 1);
