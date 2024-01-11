#ifndef SAVECODEPANE_H
#define SAVECODEPANE_H

#include <QObject>
#include <QLabel>
#include <QColor>
#include <QFont>
#include <QPushButton>
#include <QCheckBox>
#include <QWidget>
#include <QVBoxLayout>
#include <QGridLayout>
#include <string>
#include <cstddef>


using namespace std;
class SavecodePane: public QWidget{
private:
    QLabel label1;
    QLabel label2;
    QLabel codeLabel;
    QCheckBox* btReveal = nullptr;
    QPushButton* btRegenerate = nullptr;
    bool isCodeHidden = true;
    const int saveCodeLength = 6;
    //layout data fields
    QVBoxLayout* vbox = nullptr; //high level vbox;
    QGridLayout* codeBox = nullptr;
    string currCode;

    void setLayoutManagement();
public:
    SavecodePane(const string username);
    string generateCode();
    static const string CODE_HIDDEN_TEXT;
public slots:
    void btRevealHandler(bool checked);
    void btRegenHandler();
};

#endif // SAVECODEPANE_H
