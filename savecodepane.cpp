#include "savecodepane.h"

SavecodePane::SavecodePane(const string username){
    this->btReveal = new QPushButton(QString("reveal"));
    this->btRegenerate = new QPushButton(QString("regenerate"));
    this->label1.setText(QString("Your privacy is our main concern"));

    string labelHeader2 = username + ", we will provide you a saving code.\nYou may need this code to reset your password.\nPlease ensure that you write it somewhere safe";
    this->label2.setText(QString::fromStdString(labelHeader2));
    this->codeLabel.setText(QString::fromStdString(CODE_HIDDEN_TEXT));
    this->codeLabel.setStyleSheet(QString::fromStdString("border: 2px solid black;"));

    this->setLayout();

    QObject::connect(this->btReveal, &QPushButton::clicked, this, &SavecodePane::btRevealHandler);
    QObject::connect(this->btRegenerate, &QPushButton::clicked, this, &SavecodePane::btRegenHandler);
}
void SavecodePane::setLayout(){
    //instantiate the layouts
    this->vbox = new QVBoxLayout(this);
    this->codeBox = new QGridLayout(this);

    this->vbox->addWidget(&this->label1);
    this->vbox->addWidget(&this->label2);

    this->codeBox->addWidget(&this->codeLabel, 0, 0, 2, 1);
    this->codeBox->addWidget(this->btReveal, 0, 1);
    this->codeBox->addWidget(this->btRegenerate, 1, 1);
}
void SavecodePane::btRevealHandler(){

}
string SavecodePane::generateCode(){
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
    return string(chars);
}
void SavecodePane::btRegenHandler(){
    //Regenerate the saving code
}
const string SavecodePane::CODE_HIDDEN_TEXT = "******";
