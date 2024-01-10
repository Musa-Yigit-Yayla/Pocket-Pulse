#include "savecodepane.h"

SavecodePane::SavecodePane(const string username, const string savecode){
    this->btReveal = new QPushButton(QString("reveal"));
    this->label1.setText(QString("Your privacy is our main concern"));

    string labelHeader2 = username + ", we will provide you a saving code.\nYou may need this code to reset your password.\nPlease ensure that you write it somewhere safe";
    this->label2.setText(QString::fromStdString(labelHeader2));
    this->codeLabel.setText(QString::fromStdString(CODE_HIDDEN_TEXT));


    QObject::connect(this->btReveal, &QPushButton::clicked, this, &SavecodePane::btRevealHandler);
    QObject::connect(this->btRegenerate, &QPushButton::clicked, this, &SavecodePane::btRegenHandler);
}
void SavecodePane::btRevealHandler(){

}
void SavecodePane::btRegenHandler(){
    //Regenerate the saving code
}
const string SavecodePane::CODE_HIDDEN_TEXT = "******";
