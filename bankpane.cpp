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

    //initialize the description grid
    this->descriptionGrid = new QGridLayout(this);
    this->descriptionGrid->setHorizontalSpacing(BankPane::GRID_HOR_SPACING);

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

    this->sa.setLayout(this->accountsBox);
    this->sa.setFrameStyle(QFrame::NoFrame);
    //this->sa.setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    this->sa.setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
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

    accountRowBox->addWidget(currIdLabel);
    accountRowBox->addWidget(currNameLabel);
    accountRowBox->addWidget(currBalanceLabel);

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

                               for(int i = 0; i < children.size(); i++){
                                   QObject* currChild = children.at(i);

                                   QHBoxLayout* hboxPtr = qobject_cast<QHBoxLayout*>(currChild);
                                   if(hboxPtr != NULL){

                                       QObject* currAccRowChild = hboxPtr->children().at(0);
                                       int childId = stoi(qobject_cast<QLabel*>(currAccRowChild)->text().toStdString());
                                       if(childId > id){

                                           //insert the new account row right before the current one
                                           children.insert(i, newAccountRow);
                                           inserted = true;
                                       }
                                   }
                               }
                               if(!inserted){
                                   //append to the end
                                   children.append(newAccountRow);
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
const QColor BankPane::GOLDEN_COLOR = QColor(212, 175, 55);
