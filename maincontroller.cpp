#include "maincontroller.h"
#include "user.h"
#include "expensepane.h"
#include <iostream>
#include <climits>

//expects the compared column to have QVariant objects which wrap an int
struct VariantColumnComparator{
    int column;
    bool ascending;

    VariantColumnComparator(int colIndex, bool ascending = false): column{colIndex}, ascending{ascending}{

    }
    //Perform operator overloading with signature () for providing a functor
    bool operator()(const vector<QVariant>& v1, const vector<QVariant>& v2) const{
        bool result = v1.at(column).toInt() < v2.at(column).toInt();
        if(ascending){
            result = v1.at(column).toInt() > v2.at(column).toInt();
        }
        return result;
    }
};

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
    delete sq;
    return names;
}
int MainController::getUserCount(){
    QSqlQuery* sq = new QSqlQuery(this->db);
    sq->prepare(QString::fromStdString("SELECT * FROM " + USER_TABLE_NAME + ";"));
    int count = 0;
    if(sq->exec() && sq->next()){ //the first next call reaches to the 0th index if execution is successfull
        while(sq->next()){
            count++;
        }
    }
    delete sq;
    return count;
}
bool MainController::updatePassword(string username, string password){
    QSqlQuery query(this->db);
    query.prepare(QString::fromStdString("UPDATE " + USER_TABLE_NAME + " SET password = :passvalue WHERE name = :uservalue;"));
    query.bindValue(":passvalue", QString::fromStdString(password));
    query.bindValue(":uservalue", QString::fromStdString(username));

    return query.exec();
}
int MainController::getUserId(string username){
    int result = -1;
    QSqlQuery sq(this->db);
    sq.prepare(QString::fromStdString("SELECT id FROM " + USER_TABLE_NAME + " WHERE name = :username;"));
    sq.bindValue(":username", QString::fromStdString(username));

    if(sq.exec() && sq.next()){
        result = sq.value(0).toInt();
    }
    return result;
}
void MainController::createMonthlyExpenseGoalsTable(){
    if(!this->tableExists(ExpensePane::MONTHLY_GOALS_TABLENAME)){
        //create the corresponding table
        QSqlQuery sq(this->db);
        sq.prepare(QString::fromStdString("CREATE TABLE " + ExpensePane::MONTHLY_GOALS_TABLENAME + " (id INTEGER PRIMARY KEY AUTOINCREMENT, user_name TEXT, month INTEGER, year INTEGER, health INTEGER, education INTEGER, market_grocery INTEGER, entertainment INTEGER, vehicle INTEGER, fees INTEGER, other INTEGER);"));
        sq.exec();
    }

}
//Returns a bool value indicating whether a match in the monthly expense goals table has found with the given userid
bool MainController::monthlyExpenseGoalsExist(string userName){
    QSqlQuery sq(this->db);
    sq.prepare(QString::fromStdString("SELECT * FROM " + ExpensePane::MONTHLY_GOALS_TABLENAME + " WHERE user_name = :userName;"));
    sq.bindValue(":userName", QString::fromStdString(userName));

    bool success = sq.exec();
    bool found = success && sq.next();

    return found;
}
//values vector will expectedly have monthly goal values specified by the user, sentinel -1 denotes that no goal has been specified for that category
bool MainController::registerUserMonthlyGoals(string username, int month, int year, vector<int>& values){
    //if a tuple with the primary keys username, month, and year do not exist, create one
    QSqlQuery sq(this->db);
    sq.prepare(QString::fromStdString("SELECT * FROM " + ExpensePane::MONTHLY_GOALS_TABLENAME + " WHERE (user_name = :userName AND month = :month AND year = :year);"));
    sq.bindValue(":userName", QString::fromStdString(username));
    sq.bindValue(":month", month);
    sq.bindValue(":year", year);

    bool entryFound = sq.exec() && sq.next();
    bool success = false;
    if(entryFound){
        //update the entry
        string updateSubstr = "";
        for(int i = 0; i < values.size(); i++){
            if(values.at(i) != -1){
                string currStr = monthly_goal_categories_columns.at(i) + " = :value" + to_string(i);
                if(i != values.size() - 1){
                    currStr += ", ";
                }
                updateSubstr += currStr;
            }
        }
        if(updateSubstr != ""){
            sq.prepare(QString::fromStdString("UPDATE " + ExpensePane::MONTHLY_GOALS_TABLENAME + " SET " + updateSubstr + " WHERE user_name = :userName AND month = :month AND year = :year;"));
            sq.bindValue(":userName", QString::fromStdString(username));
            sq.bindValue(":month", month);
            sq.bindValue(":year", year);

            for(int i = 0; i < values.size(); i++){
                sq.bindValue(QString::fromStdString(":value" + to_string(i)), values.at(i));
            }

            success = sq.exec();
            qDebug() << "Debug: registerUserMonthlyGoals UPDATE query has been executed with success " << success;
        }
    }
    else{
        string columnsSubstr = "";
        string valuesSubstr = "";
        for(int i = 0; i < values.size(); i++){
            if(values.at(i) != -1){
                columnsSubstr += monthly_goal_categories_columns.at(i);
                valuesSubstr += ":value" + to_string(i);

                if(i != values.size() - 1){
                    columnsSubstr += ", ";
                    valuesSubstr += ", ";
                }
            }
        }
        if(columnsSubstr != "" && valuesSubstr != ""){
            sq.prepare(QString::fromStdString("INSERT INTO " + ExpensePane::MONTHLY_GOALS_TABLENAME + " (user_name, month, year, " + columnsSubstr + ") VALUES (:username, :month, :year, " + valuesSubstr + ");"));
            sq.bindValue(":username", QString::fromStdString(username));
            sq.bindValue(":month", month);
            sq.bindValue(":year", year);

            for(int i = 0; i < values.size(); i++){
                QString placeholder = QString::fromStdString(":value" + to_string(i));
                sq.bindValue(placeholder, values.at(i));
            }

            success = sq.exec();
            qDebug() << "Debug: registerUserMonthlyGoals INSERT INTO query has been executed with success " << success;
            qDebug() << "Debug: lastError in the query is: " << sq.lastError();
        }
    }
    return success;
}
vector<int> MainController::getUserMonthlyGoals(string username, int month, int year){
    vector<int> result;
    QSqlQuery sq(this->db);
    sq.prepare(QString::fromStdString("SELECT * FROM " + ExpensePane::MONTHLY_GOALS_TABLENAME + " WHERE user_name = :username AND month = :month AND year = :year;"));
    sq.bindValue(":username", QString::fromStdString(username));
    sq.bindValue(":month", month);
    sq.bindValue(":year", year);

    bool success = sq.exec();

    if(success && sq.next()){
        int startIndex = 4;
        int counter = 0;
        while(counter < (int)ExpensePane::EXPENSE_CATEGORIES::count){
            int currIndex = startIndex + counter;
            int currExpenseLimit = sq.value(currIndex).toInt();
            result.push_back(currExpenseLimit);
            counter++;
        }
    }
    else if(!success){
        qDebug() << "Debug: query execution in getUserMonthlyGoals has given errors " << sq.lastError();
    }

    return result;
}
bool MainController::createUserContactsTable(){
    bool success = false;

    if(!this->tableExists(USER_CONTACTS_TABLE_NAME)){
        QSqlQuery sq(this->db);
        sq.prepare(QString::fromStdString("CREATE TABLE " + USER_CONTACTS_TABLE_NAME + " (id INTEGER PRIMARY KEY AUTOINCREMENT, user_name, contact_name, category, explanation);"));
        success = sq.exec();
    }
    return success;
}
vector<vector<QString>> MainController::retrieveContacts(string& username){
    QSqlQuery sq(this->db);
    sq.prepare(QString::fromStdString("SELECT contact_name, category, explanation FROM " + USER_CONTACTS_TABLE_NAME + " WHERE (user_name = :username);"));
    sq.bindValue(":username", QString::fromStdString(username));

    vector<vector<QString>> result;
    if(sq.exec()){
        int columnCount = 3;
        while(sq.next()){
            vector<QString> currRow;
            for(int i = 0; i < columnCount; i++){
                currRow.push_back(sq.value(i).toString());
            }
            result.push_back(currRow);
        }
    }
    else{
        qDebug() << "Debug: latest retrieveContacts query error is: " << sq.lastError();
    }
    return result;
}
bool MainController::addContact(string username, string contactName, string category, string explanation){
    bool success = false;
    if(!this->userHasContact(username, contactName)){
        QSqlQuery sq(this->db);
        sq.prepare(QString::fromStdString("INSERT INTO " + USER_CONTACTS_TABLE_NAME + " (user_name, contact_name, category, explanation) VALUES(:username, :contactName, :category, :explanation);"));
        sq.bindValue(":username", QString::fromStdString(username));
        sq.bindValue(":contactName", QString::fromStdString(contactName));
        sq.bindValue(":category", QString::fromStdString(category));
        sq.bindValue(":explanation", QString::fromStdString(explanation));
        success = sq.exec();
    }
    return success;
}
bool MainController::userHasContact(string username, string contactName){
    QSqlQuery sq(this->db);
    sq.prepare(QString::fromStdString("SELECT * FROM " + USER_CONTACTS_TABLE_NAME + " WHERE (user_name = :username AND contact_name = :contactName);"));
    sq.bindValue(":username", QString::fromStdString(username));
    sq.bindValue(":contactName", QString::fromStdString(contactName));

    bool success = sq.exec() && sq.next();
    return success;
}
bool MainController::updateContact(string username, string contactName, string newContactName, string category, string explanation){
    QSqlQuery sq(this->db);
    sq.prepare(QString::fromStdString("UPDATE " + USER_CONTACTS_TABLE_NAME + " SET contact_name = :newCname, category = :category, explanation = :explanation WHERE (user_name = :username AND contact_name = :oldCname);"));
    sq.bindValue(":newCname", QString::fromStdString(newContactName));
    sq.bindValue(":category", QString::fromStdString(category));
    sq.bindValue(":explanation", QString::fromStdString(explanation));
    sq.bindValue(":username", QString::fromStdString(username));
    sq.bindValue(":oldCname", QString::fromStdString(contactName));

    bool success = sq.exec();
    return success;
}
bool MainController::deleteContact(string username, string contactName){
    QSqlQuery sq(this->db);
    sq.prepare(QString::fromStdString("DELETE FROM " + USER_CONTACTS_TABLE_NAME + " WHERE (user_name = :username AND contact_name = :contactName);"));
    sq.bindValue(":username", QString::fromStdString(username));
    sq.bindValue(":contactName", QString::fromStdString(contactName));

    bool success = sq.exec();
    return success;
}
int MainController::registerDebt(string username, string owedName, int amount, string explanation, string due_date, int paid_status){
    int registeredId = -1;
    bool success;
    if(!this->tableExists(USER_DEBTS_TABLE_NAME)){
        //create the user debts table
        QSqlQuery sq(this->db);
        sq.prepare(QString::fromStdString("CREATE TABLE " + USER_DEBTS_TABLE_NAME + " (id INTEGER PRIMARY KEY AUTOINCREMENT, username TEXT, owedName TEXT, amount INTEGER," +
                                          " explanation TEXT, due_date TEXT, paid_status TEXT, priority INTEGER);"));
        success = sq.exec();
    }
    QSqlQuery sq(this->db);
    //before inserting the new tuple, retrieve the highest priority number
    sq.prepare(QString::fromStdString("SELECT MAX(priority) FROM " + USER_DEBTS_TABLE_NAME + ";"));
    int currPriority = -1;
    if(sq.exec()){
        if(sq.next()){
            currPriority = sq.value(0).toInt() + 1;
        }
        else{
            currPriority = 1; // the very first tuple will be inserted
        }
    }

    sq.prepare(QString::fromStdString("INSERT INTO " + USER_DEBTS_TABLE_NAME + " (username, owedName, amount, explanation, due_date, paid_status, priority)" +
                                      " VALUES(:username, :owedName, :amount, :explanation, :due_date, :paid_status, :priority);"));
    sq.bindValue(":username", QString::fromStdString(username));
    sq.bindValue(":owedName", QString::fromStdString(owedName));
    sq.bindValue(":amount", amount);
    sq.bindValue(":explanation", QString::fromStdString(explanation));
    sq.bindValue(":due_date", QString::fromStdString(due_date));
    sq.bindValue(":paid_status", paid_status);
    sq.bindValue(":priority", currPriority);

    success = sq.exec();
    if(success){
        //retrieve the newly registered id by using aggregate max
        sq.prepare(QString::fromStdString("SELECT id FROM " + USER_DEBTS_TABLE_NAME + " WHERE id = (SELECT MAX(id) FROM " + USER_DEBTS_TABLE_NAME + ");"));
        bool idRetrieved = sq.exec();
        if(idRetrieved && sq.next()){
            registeredId = sq.value(0).toInt();
        }
    }
    return registeredId;

}
//selectCurrOnly specifies whether user wants to view only debts with paid status 1 ( current debts)
//Returns a 2d vector containing all attributes (including id and username) in each row
//The returned vector is sorted wrt debt priorites
vector<vector<QVariant>> MainController::getAllDebts(string username, bool selectCurrOnly){
    QSqlQuery sq(this->db);
    sq.prepare(QString::fromStdString("SELECT * FROM " + USER_DEBTS_TABLE_NAME + " WHERE username = :username;"));
    sq.bindValue(":username", QString::fromStdString(username));

    vector<vector<QVariant>> result;
    if(sq.exec()){
        int columnCount = 8;
        while(sq.next()){
            if(selectCurrOnly && sq.value(6).toInt() == 1){
                continue; //skip the paid debt
            }
            vector<QVariant> currRow;
            for(int i = 0; i < columnCount; i++){
                currRow.push_back(sq.value(i));
            }
            result.push_back(currRow);
        }
        //sort the 2d vector by using heap sort
        std::make_heap(result.begin(), result.end(), VariantColumnComparator(columnCount - 1));
        std::sort_heap(result.begin(), result.end(), VariantColumnComparator(columnCount - 1));
    }

    return result;
}
//returns all attributes (except debt) in the column declaration order as a vector of QString (notice the first attribute is the username)
vector<QVariant> MainController::getDebt(int debtId){
    vector<QVariant> result;
    QSqlQuery sq(this->db);
    sq.prepare(QString::fromStdString("SELECT * FROM " + USER_DEBTS_TABLE_NAME + " WHERE id = :debtId;"));
    sq.bindValue(":debtId", debtId);

    if(sq.exec() && sq.next()){
        int columnCount = 8; //including id
        for(int i = 1; i < columnCount; i++)
        result.push_back(sq.value(i));
    }
    return result;
}
//Returns INT_MAX if no such debt exists
int MainController::getDebtPriority(int debtId){
    int result = INT_MAX;
    QSqlQuery sq(this->db);
    sq.prepare(QString::fromStdString("SELECT priority FROM " + USER_DEBTS_TABLE_NAME + " WHERE id = :debtId;"));
    sq.bindValue(":debtId", debtId);
    if(sq.exec() && sq.next()){
        result = sq.value(0).toInt();
    }
    return result;
}
bool MainController::setDebtPriority(int debtId, int debtPriority){
    bool updated = false;

    QSqlQuery sq(this->db);
    sq.prepare(QString::fromStdString("UPDATE " + USER_DEBTS_TABLE_NAME + " SET priority = :debtPriority WHERE id = :debtId"));
    sq.bindValue(":debtId", debtId);
    sq.bindValue(":debtPriority", debtPriority);
    updated = sq.exec();
    return updated;
}
//Returns true when an unpaid debt is marked as paid in the database
bool MainController::markDebtAsPaid(int debtId){
    bool updated = false;

    QSqlQuery sq(this->db);
    sq.prepare(QString::fromStdString("UPDATE " + USER_DEBTS_TABLE_NAME + " SET paid_status = 1 WHERE id = :debtId AND paid_status = 0;"));
    sq.bindValue(":debtId", debtId);
    updated = sq.exec();
    return updated;
}
bool MainController::registerFinancialGoal(string username, string explanation, string dateOfCreation){
    bool created = false;
    QSqlQuery sq(this->db);
    if(!this->tableExists(FINANCIAL_GOALS_TABLE_NAME)){
        //create the financial goals table
        sq.prepare(QString::fromStdString("CREATE TABLE " + FINANCIAL_GOALS_TABLE_NAME + " (ID INTEGER PRIMARY KEY AUTOINCREMENT, userID INTEGER, "
                                                                                         "explanation TEXT, date TEXT, status INTEGER);"));
        sq.exec();
    }
    int userID = this->getUserId(username);
    sq.prepare(QString::fromStdString(
        "INSERT INTO " + FINANCIAL_GOALS_TABLE_NAME + " (userID, explanation, date, status) VALUES(:userID, :explanation, :date, 0);"));
    sq.bindValue(":userID", userID);
    sq.bindValue(":explanation", QString::fromStdString(explanation));
    sq.bindValue(":date", QString::fromStdString(dateOfCreation));

    created = sq.exec();
    return created;
}
//reached specifies that whether tuples with status 1 (reached goals) will be selected
unordered_map<int, string> MainController::retrieveFinancialGoals(string username, bool reached){
    unordered_map<int, string> result;

    if(this->tableExists(FINANCIAL_GOALS_TABLE_NAME)){
        int userID = this->getUserId(username);
        int desiredStatus = reached;

        //retrieve explanation fields of the financial goals (if exists) sorted by their creation date
        QSqlQuery sq(this->db);
        sq.prepare(QString::fromStdString("SELECT ID, explanation FROM " + FINANCIAL_GOALS_TABLE_NAME + " WHERE userID = :userID AND status = :status ORDER BY date DESC;"));
        sq.bindValue(":userID", userID);
        sq.bindValue(":status", desiredStatus);

        if(sq.exec()){
            while(sq.next()){
                //result[sq.value(0).toInt()] = sq.value(1).toString().toStdString();
                result.insert(make_pair(sq.value(0).toInt(), sq.value(1).toString().toStdString()));
            }
        }
    }
    return result;
}
bool MainController::markFinancialGoalDone(int goalID){
    bool success = false;
    QSqlQuery sq(this->db);
    sq.prepare(QString::fromStdString("UPDATE " + FINANCIAL_GOALS_TABLE_NAME + " SET status = 1 WHERE ID = :goalID;")); //mark as done
    sq.bindValue(":goalID", goalID);
    success = sq.exec();
    return success;
}
const string MainController::DB_NAME = "PocketPulseDB";
const string MainController::DB_USERNAME = "root";
const string MainController::DB_PASSWORD = "123456";

const string MainController::USER_TABLE_NAME = "User";
const string MainController::USER_CONTACTS_TABLE_NAME = "user_contacts";
const string MainController::USER_DEBTS_TABLE_NAME = "USER_DEBTS";
const string MainController::FINANCIAL_GOALS_TABLE_NAME = "financial_goals";
const vector<string> MainController::monthly_goal_categories_columns = {"health", "education", "market_grocery", "entertainment", "vehicle", "fees", "other"};
