#include "user.h"

User::User()
{

}
void User::createUser(){
    if(User::user == nullptr){
        //create the user for the first time during registration
    }
    else{
        //user already exists
    }
}

User* User::user = nullptr;
