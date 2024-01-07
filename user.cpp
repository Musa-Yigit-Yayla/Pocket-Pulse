#include "user.h"
#include <string>

using namespace std;
User::User()
{

}
void User::createUser(string name, string password){
    if(User::user == nullptr){

        //create the user for the first time during registration
    }
    else{
        //user already exists
    }
}

User* User::user = nullptr;
