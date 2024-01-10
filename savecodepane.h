#ifndef SAVECODEPANE_H
#define SAVECODEPANE_H

#include <QObject>
#include <QLabel>
#include <QPushButton>
#include <QWidget>
#include <string>
#include <cstddef>


using namespace std;
class SavecodePane: public QWidget{
private:
    QLabel label1;
    QLabel label2;
    QLabel codeLabel;
    QPushButton* btReveal = nullptr;
    QPushButton* btRegenerate = nullptr;
    bool isCodeHidden = true;
public:
    SavecodePane(const string username, const string passcode);
    static const string CODE_HIDDEN_TEXT;
public slots:
    void btRevealHandler();
    void btRegenHandler();
};

#endif // SAVECODEPANE_H
