#ifndef BANKINGCONTROLLER_H
#define BANKINGCONTROLLER_H

//Controller class for manipulating and simulating bank related mock data

#include <string>
#include <QString>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <qDebug>
#include <iostream>

using namespace std;
class BankingController{
public:
    enum ACCOUNT_ATTRIBUTES {ID, FIRST_NAME, LAST_NAME, EMAIL, BANK_PASSWORD, BALANCE};
private:
    QSqlDatabase db;


    const string HOST_NAME = "root";
    const string DB_PASS = "123456";
    const string ACCOUNT_TABLE_NAME = "BANK_DATA";
    const string TRANSACTION_TABLE_NAME = "TRANSACTION_DATA";
    const string DB_NAME = "mock_banking.db";
public:
    BankingController();

    bool accountExists(int accountId);
    string getAccountAttribute(int accountId, ACCOUNT_ATTRIBUTES attribute);


};

#endif // BANKINGCONTROLLER_H
