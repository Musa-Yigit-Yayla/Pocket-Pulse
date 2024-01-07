#ifndef USER_H
#define USER_H

#include <cstddef>
#include <string>

using namespace std;
/**
 * @brief The User class
 * Will contain a single instance
 */
class User{

private:
    static User* user;
    User(); //private constructor for providing single instance
    ~User();
private: //instance data fields
    string name;
    string password;
    string saveCode; //lifetime saving code

public:
    static void createUser(string name, string password);
    static void onExit();
};
#endif // USER_H
