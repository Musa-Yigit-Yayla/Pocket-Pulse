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
    QPushButton* btReveal = nullptr;
    bool isCodeHidden = true;
public:
    SavecodePane(const string username, const string passcode);
public slots:
    void btRevealHandler();
};

#endif // SAVECODEPANE_H
