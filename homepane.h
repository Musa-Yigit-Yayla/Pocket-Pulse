#include "abstractpane.h"
#include "user.h"
#include "progresscircle.h"
#include "maincontroller.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QLabel>
#include <string>
#include <vector>
#include <unordered_map>
#include <QVariant>

using namespace std;

#ifndef HOMEPANE_H
#define HOMEPANE_H

class HomePane: public AbstractPane{
private:
    ProgressCircle* goalsCircle = nullptr;
    ProgressCircle* debtCircle = nullptr;

    QHBoxLayout* hbox = nullptr;
    QVBoxLayout* vbox1 = nullptr;
    QVBoxLayout* vbox2 = nullptr;
    QScrollArea* debtSA = nullptr;
    QScrollArea* goalSA = nullptr;

public:
    HomePane(User* user, QWidget* parent = nullptr);
    void refresh(); //Invoke upon selection
};

#endif // HOMEPANE_H
