#include "bankpane.h"
#include "bankingcontroller.h"
#include "mainscreen.h"
#include <iostream>

using namespace std;

BankPane::BankPane(User* user, QWidget *parent)
    : AbstractPane{user, parent}{

    this->setFormPane();
    this->setLayoutManagement();
    //this->setLayout(&this->formPane);
    this->setLayout(this->pane);
}

void BankPane::setFormPane(){
    this->accountLabel.setText("Your account id:");
    this->passLabel.setText("Your account password:");
    this->btGetAccount.setText("Submit");
    this->errorLabel.setStyleSheet("color: red;");
    this->errorLabel.setVisible(false);
    this->tfAccPass.setEchoMode(QLineEdit::Password);
    this->formPane.setContentsMargins(550, 100, 60, 400);

    this->formPane.addWidget(&this->accountLabel, 0, 0);
    this->formPane.addWidget(&this->tfAccountID, 1, 0, 1, 4);
    this->formPane.addWidget(&this->passLabel, 2, 0);
    this->formPane.addWidget(&this->tfAccPass, 3, 0, 1, 4);
    this->formPane.addWidget(&this->errorLabel);
    this->formPane.addWidget(&this->btGetAccount, 5, 3);

    QObject::connect(&this->btGetAccount, &QPushButton::clicked, this, &BankPane::slotGetAccount);
}
void BankPane::setLayoutManagement(){
    this->pane = new QHBoxLayout(this);
    this->accountsBox = new QVBoxLayout(this);
    this->accountsBox->setContentsMargins(75, 25, 25, 25);
    this->accountsBox->setSpacing(5);

    //initialize the description grid
    this->descriptionGrid = new QGridLayout(this);
    this->descriptionGrid->setHorizontalSpacing(BankPane::GRID_HOR_SPACING);
    this->descriptionGrid->setSizeConstraint(QGridLayout::SetMaximumSize);

    QLabel* sumLabel = new QLabel(this);
    sumLabel->setText("<u>Total balance:          </u>"); //underline the text with HTML
    QLabel* idLabel = new QLabel(this);
    idLabel->setText("<u>Account id</u>");
    QLabel* lastNameLabel = new QLabel(this);
    lastNameLabel->setText("<u>Last Name</u>");
    QLabel* balanceLabel = new QLabel(this);
    balanceLabel->setText("<u>Account balance</u>");

    QFont midFont;
    QFont largeFont;
    midFont.setPointSize(MID_POINT_SIZE);
    largeFont.setPointSize(LARGE_POINT_SIZE);
    sumLabel->setFont(largeFont);
    idLabel->setFont(midFont);
    lastNameLabel->setFont(midFont);
    balanceLabel->setFont(midFont);

    //this->totalSumLabel.setText("$0.0");
    this->descriptionGrid->addWidget(sumLabel, 0, 0);
    this->descriptionGrid->addWidget(&this->totalSumLabel, 0, 2);
    this->descriptionGrid->addWidget(idLabel, 1, 0);
    this->descriptionGrid->addWidget(lastNameLabel, 1, 1);
    this->descriptionGrid->addWidget(balanceLabel, 1, 2);

    this->accountsBox->addLayout(this->descriptionGrid);

    //fetch the already registered accounts of the user if any exists
    BankingController bc;
    int userId = this->getCurrentUserId();
    vector<int> registeredAccounts = bc.getAccountsOfUser(userId);

    double totalBalance = 0;
    for(int i = 0; i < registeredAccounts.size(); i++){
        int currId = registeredAccounts.at(i);
        QHBoxLayout* accountBox = this->getAccountsRowBox(currId, bc);
        this->accountsBox->addLayout(accountBox);

        //update the total balance
        string balanceStr = bc.getAccountAttribute(currId, BankingController::ACCOUNT_ATTRIBUTES::BALANCE);
        double balance = stod(balanceStr.substr(1));
        totalBalance += balance;
    }

    this->totalSumLabel.setText(QString::fromStdString("$" + to_string(totalBalance)));
    QFont fontTotal = this->totalSumLabel.font();
    fontTotal.setPointSize(BankPane::LARGE_POINT_SIZE);
    this->totalSumLabel.setFont(fontTotal);
    int r = GOLDEN_COLOR.red(), g = GOLDEN_COLOR.green(), b = GOLDEN_COLOR.blue();
    this->totalSumLabel.setStyleSheet(QString::fromStdString("color: rgb(" + to_string(r) + ", " + to_string(g) + ", " + to_string(b) + ");"));

    QWidget* intermediateWrapper = new QWidget(&this->sa);
    intermediateWrapper->setLayout(this->accountsBox);
    this->sa.setFrameStyle(QFrame::NoFrame);
    //this->sa.setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    this->sa.setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    this->sa.setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    this->sa.setWidgetResizable(true);
    this->sa.setWidget(intermediateWrapper);
    this->pane->addWidget(&this->sa);



    QWidget* formPaneWrapper = new QWidget(this);
    /*QPalette palette = formPaneWrapper->palette();
    palette.setColor(QPalette::Window, QColor::fromRgb(50, 205, 50));
    formPaneWrapper->setPalette(palette);*/
    formPaneWrapper->setStyleSheet("color: rgb(50, 205, 50);");
    formPaneWrapper->setLayout(&this->formPane);
    this->pane->addWidget(formPaneWrapper);

}
QHBoxLayout* BankPane::getAccountsRowBox(int currId, BankingController& bc){

    //the hbox will contain data types id, last name and balance
    string lastName = bc.getAccountAttribute(currId, BankingController::ACCOUNT_ATTRIBUTES::LAST_NAME);
    string balance = bc.getAccountAttribute(currId, BankingController::ACCOUNT_ATTRIBUTES::BALANCE);

    QHBoxLayout* accountRowBox = new QHBoxLayout(this);
    accountRowBox->setSpacing(BankPane::GRID_HOR_SPACING);
    QLabel* currIdLabel = new QLabel(this);
    currIdLabel->setText(QString::fromStdString(to_string(currId)));
    QLabel* currNameLabel = new QLabel(this);
    currNameLabel->setText(QString::fromStdString(lastName));
    QLabel* currBalanceLabel = new QLabel(this);
    currBalanceLabel->setText(QString::fromStdString(balance));


    QWidget* buttonsWrapper = new QWidget(this);
    QPushButton* btViewTransaction = new QPushButton(buttonsWrapper);
    QPushButton* btCloseTransaction = new QPushButton(buttonsWrapper);

    btViewTransaction->setFixedSize(TOOL_INSPECT_LENGTH, TOOL_INSPECT_LENGTH);
    QString viewPath = QString::fromStdString(MainScreen::ICONS_FOLDER_PATH + "\\inspecticon.png");
    QPixmap viewImg(viewPath);
    QRect rect = viewImg.rect();
    rect.setSize(QSize(TOOL_INSPECT_LENGTH, TOOL_INSPECT_LENGTH));
    btViewTransaction->setIcon(viewImg);

    btCloseTransaction->setFixedSize(TOOL_INSPECT_LENGTH, TOOL_INSPECT_LENGTH);
    QString closePath = QString::fromStdString(MainScreen::ICONS_FOLDER_PATH + "\\iconclose.png");
    QPixmap closeImg(closePath);
    rect = closeImg.rect();
    rect.setSize(QSize(TOOL_INSPECT_LENGTH, TOOL_INSPECT_LENGTH));
    btCloseTransaction->setIcon(closeImg);


    QHBoxLayout* buttonContainer = new QHBoxLayout(buttonsWrapper);
    buttonContainer->addWidget(btViewTransaction);
    buttonContainer->addWidget(btCloseTransaction);
    //buttonContainer->setSizeConstraint(QBoxLayout::SizeConstraint::SetFixedSize);
    btCloseTransaction->setVisible(false);
    buttonsWrapper->setLayout(buttonContainer);
    buttonsWrapper->setFixedSize(TOOL_INSPECT_LENGTH * 2 + 5, TOOL_INSPECT_LENGTH);


    accountRowBox->addWidget(currIdLabel);
    accountRowBox->addWidget(currNameLabel);
    accountRowBox->addWidget(currBalanceLabel);
    accountRowBox->addWidget(buttonsWrapper);

    this->accountIndexes.push_back(currId);

    //map the buttons with the corresponding accountRowBox
    this->inspectMap.insert(make_pair(btViewTransaction, accountRowBox));
    this->closeMap.insert(make_pair(btCloseTransaction, accountRowBox));
    this->buttonAccountIdMap.insert(make_pair(btViewTransaction, currId));
    this->transactionsVisibleMap.insert(make_pair(btViewTransaction, false));
    int maxIndex = this->buttonIndexMapMaxValue();
    this->buttonIndexMap.insert(make_pair(btViewTransaction, maxIndex + 1));

    QObject::connect(btViewTransaction, &QPushButton::clicked, this, &BankPane::viewTransactions);
    QObject::connect(btCloseTransaction, &QPushButton::clicked, this, &BankPane::closeTransactions);
    return accountRowBox;
}
//Slot to register a bank account
void BankPane::slotGetAccount(){
    string givenAccId = this->tfAccountID.text().toStdString();
    string givenPass = this->tfAccPass.text().toStdString();

    if(givenAccId == "" || givenAccId == ""){
        this->errorLabel.setText("Fields cannot be empty");
    }
    else{
        //try to locate the account with given id if it exists, then validate the given password
        int id;
        bool idCasted = false;
        try{
           id = stoi(givenAccId);
            idCasted = true;
        }
        catch(exception){
           this->errorLabel.setText("Given account id must be an integer");
           this->errorLabel.setVisible(true);
        }
        if(idCasted){
           BankingController bc;

           if(bc.accountExists(id)){
               string password = bc.getAccountAttribute(id, BankingController::ACCOUNT_ATTRIBUTES::BANK_PASSWORD);
               if(givenPass == password){
                   //check whether specified account id is already in use by some user
                   if(bc.accountRegistered(id)){
                       this->errorLabel.setText(QString::fromStdString("Specified account with id " + givenAccId + " is in use"));
                       this->errorLabel.setVisible(true);
                   }
                   else{
                       int userId = this->getCurrentUserId();
                       if(userId != -1){
                           //register the account to the relational table for user has a relation
                           bool registered = bc.registerAccountToUser(id, userId);
                           qDebug() << "Debug: user account registration yielded " << registered;
                           if(registered){
                               bool inserted = false;
                               //display the newly registered account on the accountsBox

                               //Apply linear search (since we will have other layout types) on children of accountsBox
                               QObjectList children = this->accountsBox->children();
                               QHBoxLayout* newAccountRow = this->getAccountsRowBox(id, bc);
                               //int childId = stoi(qobject_cast<QLabel*>(accLabels.at(accLabels.size() - 1))->text().toStdString());

                               int currHboxIndex = 0;
                               for(int i = 1; i < children.size(); i++){ //start the iteration from index 1 as first index is descriptionGrid
                                   QObject* currChild = children.at(i);

                                   QHBoxLayout* hboxPtr = qobject_cast<QHBoxLayout*>(currChild);
                                   if(hboxPtr != NULL){
                                       int childId = this->accountIndexes.at(currHboxIndex++);
                                       if(childId > id){
                                           //insert the new account row right before the current one
                                           this->accountsBox->addLayout(newAccountRow);
                                           inserted = true;
                                       }

                                   }
                               }
                               if(!inserted){
                                   //append to the end
                                   this->accountsBox->addLayout(newAccountRow);
                               }

                           }
                           this->errorLabel.setVisible(false);
                       }
                   }

               }
               else{
                   this->errorLabel.setText("Given password is invalid");
                   this->errorLabel.setVisible(true);
               }
           }
           else{
               this->errorLabel.setText("No such account exists with the given id");
               this->errorLabel.setVisible(true);
           }
        }
    }
}
void BankPane::viewTransactions(){
    //first retrieve the proper insertion index of the transactions pane
    QObject* eventSource = QObject::sender();
    QPushButton* senderButton = qobject_cast<QPushButton*>(eventSource);
    QHBoxLayout* targetRow = this->inspectMap.at(senderButton);
    int insertionIndex = this->buttonIndexMap.at(senderButton) + 1;

    //iterate over the elements of accountsBox, then for each element check whether the hbox contains the buttonWrapper widget with the child as the event source
    //if so you have found the insertion index as i + 1
    /*for(int i = 0; i < children.size(); i++){
        QHBoxLayout* currAccRow = qobject_cast<QHBoxLayout*>(children.at(i));

        if(currAccRow != NULL && targetRow == currAccRow){
           insertionIndex = i + 1;
           break;
        }
    }*/
    if(insertionIndex != -1 && !this->transactionsVisibleMap.at(senderButton)){ //second condition ensures that the transactions pane is not open
        //make the corresponding closeButton visible
        for(auto& it: this->closeMap){
           if(it.second == targetRow){
               it.first->setVisible(true); //set the close button's visibility to true
           }
        }

        //instantiate the transactionsPane and after wrapping it into QSA, insert QSA at the insertionındex
        const int TPANE_MAX_HEIGHT = 150;
        QScrollArea* transactionWrapper = new QScrollArea(this);
        transactionWrapper->setMinimumHeight(150);
        transactionWrapper->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        transactionWrapper->setStyleSheet("background-color: rgb(145, 224, 255);");

        QGridLayout* transactionsPane = new QGridLayout(transactionWrapper);
        QLabel* contactLabel = new QLabel(transactionWrapper);
        QLabel* amountLabel = new QLabel(transactionWrapper);
        QLabel* dateLabel = new QLabel(transactionWrapper);

        BankingController bc;
        contactLabel->setText("Contact name");
        amountLabel->setText("Amount");
        dateLabel->setText("Date");

        transactionsPane->addWidget(contactLabel, 0, 0);
        transactionsPane->addWidget(amountLabel, 0, 1);
        transactionsPane->addWidget(dateLabel, 0, 2);

        int accountId = this->buttonAccountIdMap.at(senderButton);
        vector<vector<string>> transactions = bc.getPastTransactions(accountId);
        BankPane::sortTransactions(transactions);


        int rowIndex = 1;
        for(int i = transactions.size() - 1; i >= 0; i--){
           int currAccId;
           QLabel* sentStatusLabel = new QLabel(transactionWrapper);
           if(transactions.at(i).at(0) != to_string(accountId)){
               currAccId = stoi(transactions.at(i).at(0)); // contact id is receiver id
               sentStatusLabel->setText("Sent");
               sentStatusLabel->setStyleSheet("color: rgb(255, 0, 0);");
           }
           else{
               currAccId = stoi(transactions.at(i).at(2)); //contact id is sender id
               sentStatusLabel->setText("Received");
               sentStatusLabel->setStyleSheet("color: rgb(5, 120, 72);");
           }
           vector<string> contactName = bc.getFullNameByAccId(currAccId);
           QLabel* nameLabel = new QLabel(transactionWrapper);
           nameLabel->setText(QString::fromStdString(to_string(contactName.at(0).at(0)) + " " + contactName.at(1)));

           QLabel* balanceLabel = new QLabel(transactionWrapper);
           string balanceStr = transactions.at(i).at(1);
           balanceLabel->setText(QString::fromStdString(balanceStr));

           QLabel* dateLabel = new QLabel(transactionWrapper);
           dateLabel->setText(QString::fromStdString(transactions.at(i).at(3)));

           transactionsPane->addWidget(nameLabel, rowIndex, 0);
           transactionsPane->addWidget(balanceLabel, rowIndex, 1);
           transactionsPane->addWidget(dateLabel, rowIndex, 2);
           transactionsPane->addWidget(sentStatusLabel, rowIndex, 3);

           rowIndex++;
        }
        transactionWrapper->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        QWidget* midWrapper = new QWidget(transactionWrapper);
        midWrapper->setLayout(transactionsPane);
        transactionWrapper->setWidget(midWrapper);


        //insert the transactionWrapper at the insertionIndex of accountsBox
        this->accountsBox->insertWidget(insertionIndex, transactionWrapper);
        //this->updateAccountsBox();
        //this->accountsBox->insertStretch(insertionIndex + 1, 1);
        this->transactionsVisibleMap.at(senderButton) = true; //mark the corresponding map entry as true
        //update the values of buttonIndexMap by incrementing by one
        this->updateButtonIndexMap(insertionIndex, true);

        /*//iterate through each widget after insertionIndex to adjust their height programmatically
        for(int i = insertionIndex + 1; i < this->accountsBox->count(); i++){

        }*/

        qDebug() << "Debug: count of children of accountsBox is " << this->accountsBox->count();


    }
    else{
        qDebug() << "Debug: insertion index could not be retrieved by BankPane::viewTransactions or the transaction pane is already open";
    }

}
void BankPane::updateAccountsBox(){
    vector<QLayoutItem*> layoutItems;

    for(int i = 1; i < this->accountsBox->count(); i++){
        layoutItems.push_back(this->accountsBox->takeAt(i));
    }
    this->accountsBox->setSpacing(10);
    for(QLayoutItem* it: layoutItems){
        if(QWidget* widget = it->widget()){
           this->accountsBox->addWidget(widget);
        }
        else if(QLayout* layout = it->layout()){
           this->accountsBox->addLayout(layout);
        }
    }
}
//startIndex is inclusive
void BankPane::shiftAccountsBoxContent(int insertedHeight, int startIndex){

    if(insertedHeight > 0){
        for(int i =  startIndex; i < this->accountsBox->count(); i++){
           QLayoutItem* item = this->accountsBox->itemAt(i);

        }
    }
    else if(insertedHeight != 0){

    }

}
void BankPane::closeTransactions(){
    QObject* sender = QObject::sender();
    QPushButton* btSender = qobject_cast<QPushButton*>(sender);
    QPushButton* btView = this->getViewByCloseButton(btSender);

    int panelIndex = this->buttonIndexMap.at(btView) + 1;
    //close the corresponding transactionspane (delete it)
    delete this->accountsBox->children().at(panelIndex);
    btSender->setVisible(false);
    //update the map indexes
    this->updateButtonIndexMap(panelIndex, false);
    this->transactionsVisibleMap.at(btView) = false;
}
//given a close button will return the corresponding view button contained within the same layout
inline QPushButton* BankPane::getViewByCloseButton(QPushButton* btClose){
    QHBoxLayout* container = this->closeMap.at(btClose);
    QPushButton* result = nullptr;

    for(auto it = this->inspectMap.begin(); it != this->inspectMap.end(); it++){
        if(it->second == container){
           result = it->first;
        }
    }
    return result;
}
int BankPane::getCurrentUserId() const{
    //provide authorization with the account and add the account
    QWidget* mainScreenWrapper = qobject_cast<QWidget*>(this->parent());
    MainScreen* ms = nullptr;
    MainController* mainController = nullptr;
    int result = -1;
    if(mainScreenWrapper != NULL){
        ms = reinterpret_cast<MainScreen*>(mainScreenWrapper->layout());
    }
    if(ms != NULL){
        mainController = ms->getMainController();
    }
    if(mainController != NULL){
        result = mainController->getUserId(ms->getUser()->getUserName());
    }
    return result;
}
//Static method to sort the given transaction 2d vector with respect to transaction dates
//transaction dates are assumed to be at index 4 of a row
void BankPane::sortTransactions(vector<vector<string>>& transactions){
    BankPane::sortTransactionsHelper(transactions, 0, transactions.size() - 1);
}
//high index is inclusive
void BankPane::sortTransactionsHelper(vector<vector<string>>& transactions, int low, int high){
    if(low < high){
        int index = partitionTransactions(transactions, low, high);
        sortTransactionsHelper(transactions, low, index - 1);
        sortTransactionsHelper(transactions, index + 1, high);
    }
}
int BankPane::partitionTransactions(vector<vector<string>>& transactions, int low, int high){

    if(low == high - 1){
        int index = low;
        string lowElt = transactions.at(low).at(3);
        string highElt = transactions.at(high).at(3);

        if(dateCompare(lowElt, highElt) > 0){
           vector<string> temp = transactions.at(low);
           transactions.at(low) = transactions.at(high);
           transactions.at(high) = temp;
        }
        return index;
    }
    else{
        //take index low as the partition index
        string partitionElt = transactions.at(low).at(3);
        int partitionIndex = low;
        low += 1;

        while(low < high){
           string lowElt = transactions.at(low).at(3);
           while(dateCompare(lowElt, partitionElt) < 0 && low <= high){
               lowElt = transactions.at(low).at(3);
               low++;
            }
            string highElt = transactions.at(high).at(3);
            while(dateCompare(highElt, partitionElt) > 0 && low <= high){
               highElt = transactions.at(high).at(3);
               high--;
            }
            if(low < high){
               //swap the high index with low index
               vector<string> temp = transactions.at(low);
               transactions.at(low) = transactions.at(high);
               transactions.at(high) = temp;
            }
        }
        //swap high with partition index
        vector<string> temp = transactions.at(partitionIndex);
        transactions.at(partitionIndex) = transactions.at(high);
        transactions.at(high) = temp;

        partitionIndex = high;
        return partitionIndex ;
    }
}
//Compares two dates with m/d/y format, first string parameter is compared with the second given parameter
//Returns -x if compared date has occured BEFORE the comparedTo date, 0 if they are the same, x if it occured after the comparedTo where x is positive integer
int BankPane::dateCompare(string compared, string comparedTo){
    vector<int> comparedVec = splitDate(compared);
    vector<int> comparedToVec = splitDate(comparedTo);

    int y0 = comparedVec.at(2), m0 = comparedVec.at(1), d0 = comparedVec.at(0);
    int y1 = comparedToVec.at(2), m1 = comparedToVec.at(1), d1 = comparedToVec.at(0);
    int result;

    if(y0 == y1 && m0 == m1 && d0 == d1){
        result = 0;
    }
    else if(y0 != y1){
        result = y0 - y1;
    }
    else if(m0 != m1){
        result = m0 - m1;
    }
    else{
        //days differ
        result = d0 - d1;
    }
    return result;
}
//Expects a date string in m/d/y format
//Returns a vector containing the {month, day, year}
//Returns an empty vector if the given string does not conform to the format
inline vector<int> BankPane::splitDate(string date){
    vector<int> slashIndexes;

    for(int i = 0; i < date.size(); i++){
        char ch = date.at(i);
        if(ch == '/'){
           slashIndexes.push_back(i);
        }
    }
    if(slashIndexes.size() == 2){
        int s0 = slashIndexes.at(0);
        int s1 = slashIndexes.at(1);
        slashIndexes.push_back(stoi(date.substr(0, s0)));
        slashIndexes.push_back(stoi(date.substr(s0 + 1, s1)));
        slashIndexes.push_back(stoi(date.substr(s1 + 1)));
    }
    return slashIndexes;
}
//returns the max value of current hashmap regarding button and index mapping
//returns 0 if map size is 0 (since indexing starts from 1)
inline int BankPane::buttonIndexMapMaxValue(){
    int result = 0;
    for(auto it = this->buttonIndexMap.begin(); it != this->buttonIndexMap.end(); it++){
        if(it->second > result){
           result = it->second;
        }
    }
    return result;
}
//Starting from values larger or equal than beginIndex, if increment is true increment each value by 1, otherwise decrement each value by 1
inline void BankPane::updateButtonIndexMap(int beginIndex, bool increment){
    for(auto it = this->buttonIndexMap.begin(); it != this->buttonIndexMap.end(); it++){
        int currIndex = it->second;
        if(currIndex >= beginIndex){
           int deltaI = 1;
           if(!increment){
               deltaI = -1;
           }
           it->second += deltaI;
        }
    }
}
const QColor BankPane::GOLDEN_COLOR = QColor(212, 175, 55);
