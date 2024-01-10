#include "savecodepane.h"

SavecodePane::SavecodePane(const string username, const string savecode){
    this->btReveal = new QPushButton(QString("reveal"));
    this->btRegenerate = new QPushButton(QString("regenerate"));
    this->btReturn = new QPushButton(QString("return to\n registration"));
    this->label1.setText(QString("Your privacy is our main concern"));

    string labelHeader2 = username + ", we will provide you a saving code.\nYou may need this code to reset your password.\nPlease ensure that you write it somewhere safe";
    this->label2.setText(QString::fromStdString(labelHeader2));
    this->codeLabel.setText(QString::fromStdString(CODE_HIDDEN_TEXT));
    this->codeLabel.setStyleSheet(QString::fromStdString("border: 2px solid black;"));

    QObject::connect(this->btReveal, &QPushButton::clicked, this, &SavecodePane::btRevealHandler);
    QObject::connect(this->btRegenerate, &QPushButton::clicked, this, &SavecodePane::btRegenHandler);
    QObject::connect(this->btReturn, &QPushButton::clicked, this, &SavecodePane::btReturnHandler);
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
void SavecodePane::btRegenHandler(){
    //Regenerate the saving code
}
void SavecodePane::btReturnHandler(){

}
const string SavecodePane::CODE_HIDDEN_TEXT = "******";
