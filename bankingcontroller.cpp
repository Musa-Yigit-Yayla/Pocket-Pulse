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
    QSqlQuery sq;
    sq.prepare(QString::fromStdString("SELECT * FROM " + this->ACCOUNT_TABLE_NAME + " WHERE account_id = :givenId"));
    sq.bindValue(":givenId", accountId);

    return sq.exec() && sq.next();
}
string BankingController::getAccountAttribute(int accountId, BankingController::ACCOUNT_ATTRIBUTES attribute){
    string columnName;

    switch(attribute){
        case BankingController::ACCOUNT_ATTRIBUTES::ID: columnName = "account_id"; break;
        case BankingController::ACCOUNT_ATTRIBUTES::FIRST_NAME: columnName = "first_name"; break;
        case BankingController::ACCOUNT_ATTRIBUTES::LAST_NAME: columnName = "last_name"; break;
        case BankingController::ACCOUNT_ATTRIBUTES::EMAIL: columnName = "email"; break;
        case BankingController::ACCOUNT_ATTRIBUTES::BANK_PASSWORD: columnName = "bank_password"; break;
        case BankingController::ACCOUNT_ATTRIBUTES::BALANCE: columnName = "balance"; break;
    }

}
