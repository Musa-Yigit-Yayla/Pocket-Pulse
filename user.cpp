#include "user.h"
#include <string>
#include <cstdlib>

using namespace std;
User::User(string name, string password, string saveCode){
    this->name = name;
    this->password = password;
    this->saveCode = saveCode;
}
User::~User(){
    //ToDo
}
string User::getPassword() const{
    return this->password;
}
string User::getUserName() const{
    return this->name;
}
string User::getSaveCode() const{
    return this->saveCode;
}
//Given password is expected to be a valid password
void User::setPassword(const string password){
    this->password = password;
}

