#include "user.h"
#include "savecodepane.h"
#include "registerscreen.h"
#include <string>
#include <cstdlib>

using namespace std;
User::User(){

}
void User::createUser(string name, string password){
    if(User::user == nullptr){
        //create the user for the first time during registration
        User::user = new User();
        User::user->name = name;
        User::user->password = password;

        //generate saving code which is of length 6 and contains letter(s) digit(s)

        //during the very initial instantion of the user, instantiate and display svp
        User::user->svp = new SavecodePane(User::user->name);

    }
}
string User::getPassword(){
    return User::user->password;
}
string User::getUserName(){
    return User::user->name;
}
//Given password is expected to be a valid password
void User::setPassword(const string password){
    User::user->password = password;
}
User* User::user = nullptr;
