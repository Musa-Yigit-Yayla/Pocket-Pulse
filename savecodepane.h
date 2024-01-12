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
    QCheckBox* btConfirmed = nullptr;
    QPushButton* btRegenerate = nullptr;
    QPushButton* btFinish = nullptr;
    bool isCodeHidden = true;
    const int saveCodeLength = 6;
    //layout data fields
    QVBoxLayout* vbox = nullptr; //high level vbox;
    QGridLayout* codeBox = nullptr;
    string currCode;

    void setLayoutManagement();
    static const int CODE_LABEL_FONT_SIZE = 30;
    static const int LABEL1_FONT_SIZE = 50;
public:
    SavecodePane(const string username);
    ~SavecodePane();
    string generateCode();
    static const string CODE_HIDDEN_TEXT;
public slots:
    void btRevealHandler(int checked);
    void btRegenHandler();
    void btFinishHandler();
    void confirmCheckHandler(int checked);
};

#endif // SAVECODEPANE_H
