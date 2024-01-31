#ifndef BANKINGCONTROLLER_H
#define BANKINGCONTROLLER_H

//Controller class for manipulating and simulating bank related mock data

#include <string>

using namespace std;
class BankingController{
public:
    enum ACCOUNT_ATTRIBUTES {ID, FIRST_NAME, LAST_NAME, EMAIL, BANK_PASSWORD, BALANCE};
private:
    const string HOST_NAME;
    const string DB_PASS = "";
    const string ACCOUNT_TABLE_NAME;
    const string TRANSACTION_TABLE_NAME;


public:
    BankingController();

    bool accountExists(int accountId);
    string getAccountAttribute(int accountId, ACCOUNT_ATTRIBUTES attribute);


};

#endif // BANKINGCONTROLLER_H
