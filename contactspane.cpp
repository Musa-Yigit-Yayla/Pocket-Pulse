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
    hbox->addItem(spacerCopy);

    //set the addPane and its contents
    this->addPane = new QVBoxLayout(this);
    this->cnameLabel = new QLabel("Contact name:", this);
    this->tfCname = new QLineEdit(this);
    this->categoryLabel = new QLabel("Category:", this);
    this->categoryBox = new QComboBox(this);
    this->textAreaExp = new QTextEdit(this);
    this->textAreaLabel = new QLabel("Description:", this);

    const int ADD_PANE_MAXWIDTH = 400;
    //this->setMaximumWidth(ADD_PANE_MAXWIDTH);
    this->tfCname->setMaximumWidth(ADD_PANE_MAXWIDTH);
    this->categoryBox->setMaximumWidth(ADD_PANE_MAXWIDTH);
    this->textAreaExp->setMaximumWidth(ADD_PANE_MAXWIDTH);
    this->textAreaExp->setLineWidth(this->textAreaExp->fontMetrics().lineSpacing() * 2);
    this->btAddContact->setFixedWidth(100);

    int taLineSpacing = this->textAreaExp->fontMetrics().lineSpacing();
    this->textAreaExp->setMaximumHeight(taLineSpacing);


    for(int i = 0; i < CONTACT_CATEGORIES.size(); i++){
        string currStr = CONTACT_CATEGORIES.at(i);
        this->categoryBox->addItem(QString::fromStdString(currStr));
    }

    this->addPane->addWidget(cnameLabel);
    this->addPane->addWidget(tfCname);
    this->addPane->addWidget(categoryLabel);
    this->addPane->addWidget(this->categoryBox);
    this->addPane->addWidget(this->textAreaLabel);
    this->addPane->addWidget(this->textAreaExp);

    QHBoxLayout* btAddWrapper = new QHBoxLayout(this);
    QSpacerItem* newSpacer = new QSpacerItem(30, 30);
    btAddWrapper->addItem(newSpacer);
    btAddWrapper->addWidget(this->btAddContact);

    this->addPane->addLayout(btAddWrapper);

    this->highLevelGrid->addLayout(this->vbox, 0, 0);
    this->highLevelGrid->addLayout(this->addPane, 0, 2);
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
const vector<string> ContactsPane::CONTACT_CATEGORIES = {"person", "group", "corporation", "family member"};
