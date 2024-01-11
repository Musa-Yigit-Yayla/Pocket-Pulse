#include "savecodepane.h"

SavecodePane::SavecodePane(const string username){
    this->currCode = this->generateCode();

    this->btReveal = new QCheckBox(QString("reveal"));
    this->btRegenerate = new QPushButton(QString("regenerate"));
    this->label1.setText(QString("Your privacy is our main concern"));

    string labelHeader2 = username + ", we will provide you a saving code.\nYou may need this code to reset your password.\nPlease ensure that you write it somewhere safe";
    this->label2.setText(QString::fromStdString(labelHeader2));
    this->codeLabel.setText(QString::fromStdString(CODE_HIDDEN_TEXT));
    this->codeLabel.setStyleSheet(QString::fromStdString("border: 2px solid black;"));

    this->setLayoutManagement();
    this->setLayout(this->vbox);

    QObject::connect(this->btReveal, &QCheckBox::clicked, this, &SavecodePane::btRevealHandler);
    QObject::connect(this->btRegenerate, &QPushButton::clicked, this, &SavecodePane::btRegenHandler);
}
void SavecodePane::setLayoutManagement(){
    //instantiate the layouts
    this->vbox = new QVBoxLayout(this);
    this->codeBox = new QGridLayout(this);

    this->vbox->addWidget(&this->label1);
    this->vbox->addWidget(&this->label2);

    this->codeBox->addWidget(&this->codeLabel, 0, 0, 2, 1);
    this->codeBox->addWidget(this->btReveal, 0, 1);
    this->codeBox->addWidget(this->btRegenerate, 1, 1);

    this->vbox->addLayout(this->codeBox);
}
void SavecodePane::btRevealHandler(bool checked){
    if(checked){
        //reveal the code
        this->codeLabel.setText(QString::fromStdString(this->currCode));
        this->codeLabel.setStyleSheet("QLabel{color: red; border: 2px solid black;}");
    }
    else{
        this->codeLabel.setText(QString::fromStdString(CODE_HIDDEN_TEXT));
        this->codeLabel.setStyleSheet("QLabel{color: black; border: 2px solid black;}");
    }
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
    this->currCode = this->generateCode();
    //redraw the text by simply invoking the btRevealHandler without changing the checked property
    bool isChecked = this->btReveal->isChecked();
    this->btRevealHandler(isChecked);
}
const string SavecodePane::CODE_HIDDEN_TEXT = "******";
