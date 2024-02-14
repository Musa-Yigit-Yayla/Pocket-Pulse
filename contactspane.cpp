#include "contactspane.h"
#include "maincontroller.h"
#include "mainscreen.h"

ContactsPane::ContactsPane(User* user, QWidget* parent): AbstractPane{user, parent}{
    this->highLevelGrid = new QGridLayout(this);

    //const int SCROLL_AREA_MIN_SIZE = 550;
    const int SCROLL_AREA_MAX_HEIGHT = 850;
    this->scrollArea = new QScrollArea(this);
    //this->scrollArea->setMinimumSize(SCROLL_AREA_MIN_SIZE, SCROLL_AREA_MIN_SIZE);
    this->scrollArea->setMaximumHeight(SCROLL_AREA_MAX_HEIGHT);
    this->scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    this->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    this->scrollArea->setWidgetResizable(true);
    this->gridPane = new QGridLayout(this);

    this->expLabel = new QLabel("Manage Contacts", this);
    this->expLabel->setStyleSheet("color: rgb(8, 40, 190); font-size: 16px;");

    this->btAddContact = new QPushButton("+", this);
    this->btAddContact->setToolTip("Add new contact");

    this->checkBoxDelete = new QCheckBox("Delete enabled", this);
    this->checkBoxDelete->setChecked(false);

    this->setLayoutManagement();

    QObject::connect(this->checkBoxDelete, &QCheckBox::stateChanged, this, &ContactsPane::cbDeleteEnableSlot);
    QObject::connect(this->btAddContact, &QPushButton::clicked, this, &ContactsPane::addContactSlot);
}
void ContactsPane::setLayoutManagement(){
    //set the vbox for containing the handshake image and expLabel
    this->vbox = new QVBoxLayout(this);
    this->imgLabel = new QLabel(this);

    const int IMAGE_LENGTH = 120;
    QImage img(QString::fromStdString(MainScreen::ICONS_FOLDER_PATH + "//handshake_icon.jpg"));
    QImage tempImg = img.scaled(IMAGE_LENGTH, IMAGE_LENGTH);
    img = tempImg;
    this->imgLabel->setPixmap(QPixmap::fromImage(img));
    QSpacerItem* spacer = new QSpacerItem(30, 30, QSizePolicy::Preferred, QSizePolicy::Preferred);

    this->vbox->addWidget(this->imgLabel);
    this->vbox->addWidget(this->expLabel);
    this->vbox->addSpacerItem(spacer);

    this->initializeGridContent();
    //add the gridPane into the scrollArea via setLayout
    this->scrollArea->setLayout(this->gridPane);

    QHBoxLayout* hbox = new QHBoxLayout(this); //hbox to contain button add and checkbox delete
    QSpacerItem* spacerCopy = new QSpacerItem(*spacer);
    hbox->addWidget(this->checkBoxDelete);
    hbox->addWidget(this->btAddContact);
    hbox->addItem(spacerCopy);

    //set the addPane
    this->addPane = new QVBoxLayout(this);


    this->highLevelGrid->addLayout(this->vbox, 0, 0);
    this->highLevelGrid->addWidget(this->scrollArea, 1, 0);
    this->highLevelGrid->addLayout(hbox, 2, 1);

}
void ContactsPane::initializeGridContent(){
    MainController mc;
    mc.createUserContactsTable();

    const int NAME_PSIZE = 15;
    string userName = this->user->getUserName();
    vector<vector<QString>> contactsInformation = mc.retrieveContacts(userName); //contains 3 labels on each row specifying cname, category, explanation
    for(int i = 0; i < contactsInformation.size(); i++){
        QLabel* currName = new QLabel(contactsInformation.at(i).at(0), this);
        QLabel* currCategory = new QLabel(contactsInformation.at(i).at(1), this);
        QLabel* currExp = new QLabel(contactsInformation.at(i).at(2), this);

        currName->setStyleSheet(QString::fromStdString("font-size: " + to_string(NAME_PSIZE) + ";"));

        this->gridPane->addWidget(currName);
        this->gridPane->addWidget(currCategory);
        this->gridPane->addWidget(currExp);
    }

}
void ContactsPane::addContactSlot(){

}
void ContactsPane::cbDeleteEnableSlot(int checked){

}
