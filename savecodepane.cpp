#include "savecodepane.h"

SavecodePane::SavecodePane(const string username, const string savecode){
    this->btReveal = new QPushButton(QString("reveal"));

    QObject::connect(this->btReveal, &QPushButton::clicked, this, &SavecodePane::btRevealHandler);
}
void SavecodePane::btRevealHandler(){

}
