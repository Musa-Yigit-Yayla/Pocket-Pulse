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
#include <QComboBox>
#include <QTextEdit>
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
    QVBoxLayout* addPane = nullptr;

    QLabel* expLabel = nullptr;
    QPushButton* btAddContact = nullptr;
    QCheckBox* checkBoxDelete = nullptr;
    QLabel* imgLabel = nullptr;

    QLabel* cnameLabel = nullptr;
    QLineEdit* tfCname = nullptr;
    QLabel* categoryLabel = nullptr;
    QComboBox* categoryBox = nullptr;
    QLabel* textAreaLabel = nullptr;
    QLineEdit* textAreaExp = nullptr;
    QLabel* errorLabel = nullptr;

    vector<QToolButton*> deleteButtons;

public:
    ContactsPane(User* user, QWidget* parent = nullptr);

    static const vector<string> CONTACT_CATEGORIES;
private:
    void setLayoutManagement();
    void initializeGridContent();
public slots:
    void cbDeleteEnableSlot(int checked);
    void addContactSlot();
};

#endif // CONTACTSPANE_H
