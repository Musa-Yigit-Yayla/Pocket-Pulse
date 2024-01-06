#ifndef USER_H
#define USER_H

#include <cstddef>

/**
 * @brief The User class
 * Will contain a single instance
 */
class User{

private:
    static User* user;
    User(); //private constructor for providing single instance
    ~User();
public:
    static void createUser();
    static void onExit();
};
#endif // USER_H
