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
#include <unordered_map>
#include <qDebug>
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

    int gridRowCount = 0;

    QLabel* expLabel = nullptr;
    QPushButton* btAddContact = nullptr;
    QCheckBox* checkBoxDelete = nullptr;
    QCheckBox* checkBoxEdit = nullptr;
    QLabel* imgLabel = nullptr;

    QLabel* cnameLabel = nullptr;
    QLineEdit* tfCname = nullptr;
    QLabel* categoryLabel = nullptr;
    QComboBox* categoryBox = nullptr;
    QLabel* textAreaLabel = nullptr;
    QLineEdit* textAreaExp = nullptr;
    QLabel* errorLabel = nullptr;

    //screen and its event sources for editing contacts pane
    QWidget* screen = nullptr;
    QLineEdit* leExp = nullptr;
    QLineEdit* leName = nullptr;
    QComboBox* cb = nullptr;
    QLabel* errLabel = nullptr;
    int currRowIndex; //current rowIndex for the clicked update button

    vector<QToolButton*> deleteButtons;
    vector<QToolButton*> editButtons;

    unordered_map<QToolButton*, int> deleteRowMap;
    unordered_map<QToolButton*, int> editRowMap;

public:
    ContactsPane(User* user, QWidget* parent = nullptr);

    static inline bool isEmpty(string& str);
    static const vector<string> CONTACT_CATEGORIES;
    static const int ICON_LENGTH = 20;
private:
    void setLayoutManagement();
    void initializeGridContent();
    void addToolButtons(int rowIndex);
    void updateIndexMaps(int deletionIndex);
public slots:
    void cbEnableSlot(int checked);
    void addContactSlot();
    void deleteContactSlot();
    void editContactSlot();
};

#endif // CONTACTSPANE_H
