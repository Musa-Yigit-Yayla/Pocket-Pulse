#ifndef USER_H
#define USER_H

#include "savecodepane.h"
#include <cstddef>
#include <string>

using namespace std;
/**
 * @brief The User class
 * Will contain a single instance
 */
class User{

private: //instance data fields
    string name;
    string password;
    string saveCode; //lifetime saving code
    string profilePicPath;

public:
    User(string name, string password, string savecode);
    ~User();

    string getUserName() const;
    string getPassword() const;
    string getSaveCode() const;
    string getProfileImagePath() const;
    void setPassword(const string password);
};
#endif // USER_H
