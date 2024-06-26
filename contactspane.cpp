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
    this->checkBoxEdit = new QCheckBox("Edit enabled", this);
    this->checkBoxEdit->setChecked(false);

    this->errorLabel = new QLabel(this);
    this->errorLabel->setVisible(false);
    this->errorLabel->setStyleSheet("color: rgb(255, 0, 0);");

    this->setLayoutManagement();

    QObject::connect(this->checkBoxDelete, &QCheckBox::stateChanged, this, &ContactsPane::cbEnableSlot);
    QObject::connect(this->checkBoxEdit, &QCheckBox::stateChanged, this, &ContactsPane::cbEnableSlot);
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
    hbox->addWidget(this->checkBoxEdit);
    hbox->addItem(spacerCopy);

    //set the addPane and its contents
    this->addPane = new QVBoxLayout(this);
    this->cnameLabel = new QLabel("Contact name:", this);
    this->tfCname = new QLineEdit(this);
    this->categoryLabel = new QLabel("Category:", this);
    this->categoryBox = new QComboBox(this);
    this->textAreaExp = new QLineEdit(this);
    this->textAreaLabel = new QLabel("Description:", this);

    const int ADD_PANE_MAXWIDTH = 400;
    //this->setMaximumWidth(ADD_PANE_MAXWIDTH);
    this->tfCname->setMaximumWidth(ADD_PANE_MAXWIDTH);
    this->categoryBox->setMaximumWidth(ADD_PANE_MAXWIDTH);
    this->textAreaExp->setMaximumWidth(ADD_PANE_MAXWIDTH);
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
    this->addPane->addWidget(this->errorLabel);

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

        this->gridPane->addWidget(currName, i, 0);
        this->gridPane->addWidget(currCategory, i, 1);
        this->gridPane->addWidget(currExp, i, 2);

        this->addToolButtons(i);

    }
    this->gridRowCount = contactsInformation.size();
    qDebug() << "Debug: gridRowCount after loop execution in ContactsPane is " << this->gridRowCount;
}
/**
 * Adds edit and delete buttons into the gridPane
 *Buttons are visible or invisible wrt curr checkbox selections
 *Makes the mapping with the given rowIndex
 *rowIndex indexing starts from 0
*/
void ContactsPane::addToolButtons(int rowIndex){
    QToolButton* btDelete = new QToolButton(this);
    QToolButton* btEdit = new QToolButton(this);

    QImage deleteImg(QString::fromStdString(MainScreen::ICONS_FOLDER_PATH + "//icondelete.png"));
    deleteImg = deleteImg.scaled(ContactsPane::ICON_LENGTH, ContactsPane::ICON_LENGTH);
    btDelete->setIcon(QPixmap::fromImage(deleteImg));

    QImage editImg(QString::fromStdString(MainScreen::ICONS_FOLDER_PATH + "//editiconwhite.png"));
    editImg = editImg.scaled(ContactsPane::ICON_LENGTH, ContactsPane::ICON_LENGTH);
    btEdit->setIcon(QPixmap::fromImage(editImg));

    QHBoxLayout* hbox = new QHBoxLayout(this);
    hbox->addWidget(btEdit);
    hbox->addWidget(btDelete);
    QSpacerItem* spacer = new QSpacerItem(30, 30);
    hbox->addSpacerItem(spacer);

    //insert the hbox into the designated gridpane position
    int insertionColumn = 3;
    this->gridPane->addLayout(hbox, rowIndex, insertionColumn);

    //adjust visibility
    bool deleteVisible = this->checkBoxDelete->isChecked();
    bool editVisible = this->checkBoxEdit->isChecked();

    btDelete->setVisible(deleteVisible);
    btEdit->setVisible(editVisible);

    //map the buttons with the rowIndex
    this->deleteRowMap.insert(make_pair(btDelete, rowIndex));
    this->editRowMap.insert(make_pair(btEdit, rowIndex));

    //this->deleteButtons.insert(this->deleteButtons.begin() + rowIndex, btDelete);
    //this->editButtons.insert(this->editButtons.begin() + rowIndex, btEdit);
    this->deleteButtons.push_back(btDelete);
    this->editButtons.push_back(btEdit);

    //set event handling signal slot connections
    QObject::connect(btDelete, &QToolButton::clicked, this, &ContactsPane::deleteContactSlot);
    QObject::connect(btEdit, &QToolButton::clicked, this, &ContactsPane::editContactSlot);
}
void ContactsPane::addContactSlot(){
    string cname = this->tfCname->text().toStdString();
    string category = this->categoryBox->currentText().toStdString();
    string explanation = this->textAreaExp->text().toStdString();

    if(cname == ""){
        this->errorLabel->setText("Contact name cannot be empty");
        this->errorLabel->setStyleSheet("color: rgb(255, 0, 0);");
        this->errorLabel->setVisible(true);
    }
    else{
        MainController mc;
        bool success = mc.addContact(this->user->getUserName(), cname, category, explanation);
        if(success){
            this->errorLabel->setVisible(false);
            //display the newly added contact on the gridPane
            QLabel* currName = new QLabel(QString::fromStdString(cname), this);
            QLabel* currCategory = new QLabel(QString::fromStdString(category), this);
            QLabel* currExp = new QLabel(QString::fromStdString(explanation), this);

            this->gridPane->addWidget(currName, this->gridRowCount, 0);
            this->gridPane->addWidget(currCategory, this->gridRowCount, 1);
            this->gridPane->addWidget(currExp, this->gridRowCount, 2);
            this->addToolButtons(this->gridRowCount);
            this->gridRowCount++;
        }
        else{
            this->errorLabel->setVisible(true);
            this->errorLabel->setText("Issue while adding");

        }
    }
}
void ContactsPane::deleteContactSlot(){
    qDebug() << "Debug: popup screen being null evaluates to " << (this->screen == NULL);
    if(this->screen == NULL){
        //allow deletion if no edit contact pane is open

        int rowIndex = this->deleteRowMap.at(qobject_cast<QToolButton*>(QObject::sender()));
        string contactName = qobject_cast<QLabel*>(this->gridPane->itemAtPosition(rowIndex, 0)->widget())->text().toStdString();

        MainController mc;
        bool deleted = mc.deleteContact(this->user->getUserName(), contactName);
        if(deleted){
            //update the gridPane accordingly

            //right before deletion update the tool button indexes (ones starting after the deletion index) by decremeting by one
            this->updateIndexMaps(rowIndex);

            int rowEltCount = 3; //number of widgets in a single row of gridPane (without edit & delete tool buttons)
            QWidget* arr[rowEltCount];
            for(int i = 0; i < rowEltCount; i++){
                arr[i] = this->gridPane->itemAtPosition(rowIndex, i)->widget();
            }
            //remove the widgets programmatically
            for(int i = 0; i < rowEltCount; i++){
                this->gridPane->removeWidget(arr[i]);
                delete arr[i];
            }

        }
        else{
            this->errorLabel->setVisible(true);
            this->errorLabel->setText("Error while deleting");
        }
    }
}
void ContactsPane::updateIndexMaps(int deletionIndex){
    QToolButton* btDelete = nullptr;
    QToolButton* btEdit = nullptr;
    for(auto& it: this->deleteRowMap){
        if(it.second > deletionIndex){
            //it.second--;
        }
        else if(it.second == deletionIndex){
            btDelete = it.first;
        }
    }
    for(auto& it: this->editRowMap){
        if(it.second > deletionIndex){
            //it.second--;
        }
        else if(it.second == deletionIndex){
            btEdit = it.first;
        }
    }
    if(btDelete != NULL && btEdit != NULL){
        //depending on the visibility of delete and edit buttons, remove the catched tool buttons if applicable
        if(this->checkBoxDelete->isChecked()){
            this->gridPane->removeWidget(btDelete);
        }
        if(this->checkBoxEdit->isChecked()){
            this->gridPane->removeWidget(btEdit);
        }

        //erase from the maps
        this->deleteRowMap.erase(btDelete);
        this->editRowMap.erase(btEdit);
        //remove from the tool button vectors
        for(int i = 0; i < this->deleteButtons.size(); i++){ //notice delete and edit button vectors' size are equal
            if(this->deleteButtons.at(i) == btDelete && this->editButtons.at(i) == btEdit){
                this->deleteButtons.erase(this->deleteButtons.begin() + i);
                this->editButtons.erase(this->editButtons.begin() + i);
                break;
            }
        }

        delete btDelete;
        delete btEdit;

        /*this->gridRowCount--;


        //lastly shift the children of the gridPane starting from deletionIndex + 1 by -1 in terms of row index
        for(int i = deletionIndex + 1; i < this->gridPane->rowCount(); i++){
            for(int j = 0; j < this->gridPane->columnCount(); j++){
                QWidget* currWidget = this->gridPane->itemAtPosition(i, j)->widget();
                if(currWidget != NULL){
                    //first remove from the current widget then shift it to (i - 1)th index
                    this->gridPane->removeWidget(currWidget);
                    this->gridPane->addWidget(currWidget, i - 1, j);
                }
            }
        }*/
    }


}
void ContactsPane::editContactSlot(){
    if(this->screen == NULL){
        //display a popup screen
        this->screen = new QWidget();

        QLabel* descLabel = new QLabel("Edit contact", screen);
        descLabel->setStyleSheet("font-size: 14px; color: rgb(189, 90, 5);");
        QPushButton* btUpdate = new QPushButton("update", screen);
        this->errLabel = new QLabel("Name cannot be empty!", screen);
        errLabel->setStyleSheet("color: rgb(255, 0, 0);");
        errLabel->setVisible(false);

        int rowIndex = this->editRowMap.at(qobject_cast<QToolButton*>(QObject::sender()));
        QLabel* nameLabel = qobject_cast<QLabel*>(this->gridPane->itemAtPosition(rowIndex, 0)->widget());
        qDebug() << "Debug: nameLabel pointer after reinterpret_cast is null evaluates to " << (nameLabel == 0);
        this->leName = new QLineEdit(nameLabel->text(), screen);

        this->cb = new QComboBox(screen);
        int currIndex = 0;
        string currSelection = qobject_cast<QLabel*>(this->gridPane->itemAtPosition(rowIndex, 1)->widget())->text().toStdString();
        for(int i = 0; i < CONTACT_CATEGORIES.size(); i++){
            string currStr = CONTACT_CATEGORIES.at(i);
            cb->addItem(QString::fromStdString(currStr));

            if(currStr == currSelection){
                currIndex = i;
            }
        }
        cb->setCurrentIndex(currIndex);

        this->leExp = new QLineEdit(qobject_cast<QLabel*>(this->gridPane->itemAtPosition(rowIndex, 2)->widget())->text(), screen);
        this->currRowIndex = rowIndex;

        QGridLayout* pane = new QGridLayout(screen);

        QObject::connect(btUpdate, &QPushButton::clicked, [&](){
            string cname = leName->text().toStdString();
            if(isEmpty(cname)){
                errLabel->setVisible(true);
            }
            else{
                //update the contact entity
                string newCategory = cb->currentText().toStdString();
                string newExp = leExp->text().toStdString();
                string username = this->user->getUserName();
                const string initialName = qobject_cast<QLabel*>(this->gridPane->itemAtPosition(this->currRowIndex, 0)->widget())->text().toStdString();

                MainController mc;
                bool updated = mc.updateContact(username, initialName, cname, newCategory, newExp);
                qDebug() << "Debug: contact credentials updated evaluated to " << updated;
                if(updated){
                    //update the contents in the contacts gridPane
                    qobject_cast<QLabel*>(this->gridPane->itemAtPosition(this->currRowIndex, 0)->widget())->setText(leName->text());
                    qobject_cast<QLabel*>(this->gridPane->itemAtPosition(this->currRowIndex, 1)->widget())->setText(cb->currentText());
                    qobject_cast<QLabel*>(this->gridPane->itemAtPosition(this->currRowIndex, 2)->widget())->setText(leExp->text());
                    //close the screen programatically and deallocate it and assign the pointer to nullptr
                    this->screen->close();
                    delete this->screen;
                    this->screen = nullptr;
                }
            }
        });
        pane->addWidget(descLabel, 0, 0);
        pane->addWidget(leName, 1, 0);
        pane->addWidget(leExp, 1, 1);
        pane->addWidget(cb, 2, 0);
        pane->addWidget(errLabel, 3, 0);
        pane->addWidget(btUpdate, 3, 1);
        screen->setLayout(pane);
        screen->show();
    }

}
bool ContactsPane::isEmpty(string& str){
    bool isEmpty = true;
    for(int i = 0; i < str.size(); i++){
        if(str.at(i) != ' '){
            isEmpty = false;
            break;
        }
    }
    return isEmpty;
}
void ContactsPane::cbEnableSlot(int checked){
    qDebug() << "Debug: ContactsPane::cbEnableSlot has been invoked with checked " << checked;
    QObject* sender = QObject::sender();

    vector<QToolButton*> temp;
    vector<QToolButton*>& buttons = temp;
    if(sender == this->checkBoxDelete){
        buttons = this->deleteButtons;
    }
    else if(sender == this->checkBoxEdit){
        buttons = this->editButtons;
    }
    for(QToolButton* bt: buttons){
        bt->setVisible(checked != 0);
    }
    qDebug() << "Debug: cbEnableSlot buttons size is " << buttons.size();
}
const vector<string> ContactsPane::CONTACT_CATEGORIES = {"person", "group", "corporation", "family member"};
