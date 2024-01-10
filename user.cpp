#include "user.h"
#include <string>
#include <cstdlib>

using namespace std;
User::User()
{

}
void User::createUser(string name, string password){
    if(User::user == nullptr){
        //create the user for the first time during registration
        User::user = new User();
        User::user->name = name;
        User::user->password = password;

        //generate saving code which is of length 6 and contains letter(s) digit(s)
        int saveCodeLength = 6;
        char chars[saveCodeLength];
        chars[0] = rand() % 10 + 48;
        chars[1] = rand() % 26 + 65; //uppercase letters
        for(int i = 2; i < saveCodeLength; i++){
            bool isLetter = rand() % 2 == 0;
            char ch;
            if(isLetter){
                ch = rand() % 26 + 65;
                if(rand() % 2 == 0){
                    //convert to lowercase
                    ch = tolower(ch);
                }
            }
            else{
                ch = rand() % 10 + 48;
            }
            chars[i] = ch;
        }
        //shuffle the generated chars
        for(int i = 0; i < saveCodeLength; i++){
            int j = rand() % saveCodeLength;

            char temp = chars[i];
            chars[i] = chars[j];
            chars[j] = temp;
        }
        User::user->saveCode = string(chars);
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
