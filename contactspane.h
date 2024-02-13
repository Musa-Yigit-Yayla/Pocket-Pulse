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
#include <QSpacerItem>
#include <cstddef>

#ifndef CONTACTSPANE_H
#define CONTACTSPANE_H

using namespace std;
class ContactsPane: public AbstractPane{
private:
    QScrollArea* scrollArea = nullptr;
    QGridLayout* gridPane = nullptr;
    QVBoxLayout* vbox = nullptr;

    QLabel* expLabel = nullptr;
    QPushButton* btAddContact = nullptr;
    QCheckBox* checkBoxDelete = nullptr;
    QLabel* imgLabel = nullptr;

public:
    ContactsPane(User* user, QWidget* parent = nullptr);
private:
    void setLayoutManagement();
public slots:
};

#endif // CONTACTSPANE_H
