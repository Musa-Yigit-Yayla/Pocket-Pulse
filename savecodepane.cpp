#include "savecodepane.h"
#include "user.h"

SavecodePane::SavecodePane(const string username){
    this->currCode = this->generateCode();

    this->btReveal = new QCheckBox(QString("reveal"));
    this->btConfirmed = new QCheckBox(QString("I have saved my lifetime save code in a secure location"));
    this->btRegenerate = new QPushButton(QString("regenerate"));
    this->btFinish = new QPushButton(QString("finish"));
    this->label1.setText(QString("Your privacy is our main concern"));
    string label1Style = "QLabel{font-size: " + to_string(SavecodePane::LABEL1_FONT_SIZE) + ";}";
    this->label1.setStyleSheet(QString::fromStdString(label1Style));

    string labelHeader2 = username + ", we will provide you a saving code.\nYou may need this code to reset your password.\nPlease ensure that you write it somewhere safe";
    this->label2.setText(QString::fromStdString(labelHeader2));
    this->codeLabel.setText(QString::fromStdString(CODE_HIDDEN_TEXT));
    string codeLabelStyle = "QLabel{border: 2px solid black; font-size: " + to_string(SavecodePane::CODE_LABEL_FONT_SIZE) + ";}";
    this->codeLabel.setStyleSheet(QString::fromStdString(""));

    this->setLayoutManagement();
    this->setLayout(this->vbox);

    QObject::connect(this->btReveal, &QCheckBox::stateChanged, this, &SavecodePane::btRevealHandler);
    QObject::connect(this->btRegenerate, &QPushButton::clicked, this, &SavecodePane::btRegenHandler);
    QObject::connect(this->btFinish, &QPushButton::clicked, this, &SavecodePane::btFinishHandler);
    QObject::connect(this->btConfirmed, &QCheckBox::stateChanged, &SavecodePane::confirmCheckHandler);
}
SavecodePane::~SavecodePane(){
    this->codeBox->removeWidget(this->btReveal);
    this->codeBox->removeWidget(this->btRegenerate);

    delete this->btReveal;
    delete this->btConfirmed;
    delete this->btRegenerate;
    delete this->btFinish;

    delete this->codeBox;
    delete this->vbox;
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
void SavecodePane::btRevealHandler(int checked){
    if(checked){
        //reveal the code
        this->codeLabel.setText(QString::fromStdString(this->currCode));
        this->codeLabel.setStyleSheet("QLabel{color: red; border: 2px solid black;}");
        this->btFinish->setVisible(true);
    }
    else{
        this->codeLabel.setText(QString::fromStdString(CODE_HIDDEN_TEXT));
        this->codeLabel.setStyleSheet("QLabel{color: black; border: 2px solid black;}");
        this->btFinish->setVisible(false);
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
void SavecodePane::btFinishHandler(){
    //proceed if the btConfirmed checkbox is checked
    if(this->btConfirmed->isChecked()){
        User::setSaveCode(this->currCode);
        this->close();
    }
    else{
        //set the stroke of the confirm checkbox to red
        this->btConfirmed->setStyleSheet("QCheckBox{border-color: red}");
    }
}
void SavecodePane::confirmCheckHandler(int checked){
    if(checked){
        //set the stroke back to normal
        this->btConfirmed->setStyleSheet("QCheckBox{border-color: black}");
    }
}
const string SavecodePane::CODE_HIDDEN_TEXT = "******";
