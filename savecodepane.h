#ifndef SAVECODEPANE_H
#define SAVECODEPANE_H

#include <QObject>
#include <QLabel>
#include <QPushButton>
#include <QWidget>
#include <QVBoxLayout>
#include <QGridLayout>
#include <string>
#include <cstddef>


using namespace std;
class SavecodePane: public QWidget{
Q_OBJECT
private:
    QLabel label1;
    QLabel label2;
    QLabel codeLabel;
    QPushButton* btReveal = nullptr;
    QPushButton* btRegenerate = nullptr;
    bool isCodeHidden = true;
    const int saveCodeLength = 6;
    //layout data fields
    QVBoxLayout* vbox = nullptr; //high level vbox;
    QGridLayout* codeBox = nullptr;

    void setLayout();
public:
    SavecodePane(const string username);
    string generateCode();
    static const string CODE_HIDDEN_TEXT;
public slots:
    void btRevealHandler();
    void btRegenHandler();
};

#endif // SAVECODEPANE_H
