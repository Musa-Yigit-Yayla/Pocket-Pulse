#include "abstractpane.h"
#include <QScrollArea>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QToolButton>
#include <QWidget>
#include <QString>
#include <QImage>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <vector>
#include <cstddef>

#ifndef CONTACTSPANE_H
#define CONTACTSPANE_H

using namespace std;
class ContactsPane: public AbstractPane{
private:
    QGridLayout* highLevelGrid = nullptr;
    QScrollArea* scrollArea = nullptr;
    QGridLayout* gridPane = nullptr;
    QVBoxLayout* vbox = nullptr;

    QLabel* expLabel = nullptr;
    QPushButton* btAddContact = nullptr;
    QCheckBox* checkBoxDelete = nullptr;
    QLabel* imgLabel = nullptr;

    vector<QToolButton*> deleteButtons;

public:
    ContactsPane(User* user, QWidget* parent = nullptr);
private:
    void setLayoutManagement();
public slots:
    void cbDeleteEnableSlot(int checked);
};

#endif // CONTACTSPANE_H
