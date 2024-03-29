#ifndef BANKINGCONTROLLER_H
#define BANKINGCONTROLLER_H

//Controller class for manipulating and simulating bank related mock data

#include <string>
#include <QString>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <qDebug>
#include <iostream>
#include <unordered_set>

using namespace std;
class BankingController{
public:
    enum ACCOUNT_ATTRIBUTES {ID, FIRST_NAME, LAST_NAME, EMAIL, BANK_PASSWORD, BALANCE};
    enum TRANSACTION_CATEGORIES {Health, Education, Grocery_Market, Entertainment, Vehicle_Oil, Fees, Other};

private:
    QSqlDatabase db;


    const string HOST_NAME = "root";
    const string DB_PASS = "123456";
    const string ACCOUNT_TABLE_NAME = "BANK_DATA";
    const string TRANSACTION_TABLE_NAME = "TRANSACTION_DATA2";
    const string DB_NAME = "mock_banking.db";
    const string USER_ACCOUNT_TABLE_NAME = "user_has_account";
public:
    BankingController();

    bool accountExists(int accountId);
    string getAccountAttribute(int accountId, ACCOUNT_ATTRIBUTES attribute);
    vector<int> getAccountsOfUser(int userId);
    bool accountRegistered(int accountId);
    bool registerAccountToUser(int accountId, int userId);
    vector<vector<string>> getPastTransactions(int accountId);
    vector<string> getFullNameByAccId(int accountId);
    vector<vector<int>> retrieveIncome(const string username);
    vector<vector<QVariant>> getSpentTransactions(const int userId, const int month, const int year, int category);
    int sumSentTransactions(int userId, int category, int month = -1, int year = -1);
    int sumAllTransactions(int userId, bool isSent, int month, int year); //sum the transactions regardless of transaction category
    vector<QString> getSpenditureMonths(const string& username); //get the months (along with year) where the user has made any spenditure transactions
    vector<int> getMaxTransactionsDateSpan(const string username); //retrieve the min and max month, year tuple where user has participated in a transaction
    vector<vector<QVariant>> getMonthlyTransactionsFromInterval(string username, int month, int year);

    static inline vector<int> splitDate(const string givenDate);
    static const QDate fromDate;
};

#endif // BANKINGCONTROLLER_H
