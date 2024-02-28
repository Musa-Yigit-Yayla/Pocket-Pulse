#include "debtpane.h"
#include "contactspane.h"
#include "maincontroller.h"
#include "bankingcontroller.h"
#include <string>

using namespace std;

DebtPane::DebtPane(User* user, QWidget* parent): AbstractPane{user, parent}{

    //initialize debtpane layout management and child widgets
    this->initializeDebtPane();
    this->gridPane->addLayout(this->vbox, 0, 0);
    this->gridPane->addWidget(this->btAddDebt, 1, 0);

    QObject::connect(this->btAddDebt, &QPushButton::clicked, this, &DebtPane::addDebtSlot);
}
//private method to initialize the contents of the debts if any exist
void DebtPane::initializeDebtPane(){
    //retrieve the debts ordered wrt their categories
    MainController mc;
    /*
    vector<vector<QString>> existingDebts = mc.getAllDebts(this->user->getUserName());
    //ToDo*/
}
void DebtPane::addDebtSlot(){

    if(this->popupDebt == NULL){ //this condition implies that each and every popup debt related data field is null hence we initialize them
        popupDebt = new QWidget();
        this->layout = new QGridLayout(popupDebt);

        this->expLabel = new QLabel("Register a debt to be paid:", popupDebt);
        expLabel->setStyleSheet("font-size: 15; color: orange;");
        this->cbContactEnabled = new QCheckBox("Select from\n   contacts", popupDebt);

        labelOwedName = new QLabel("Owed name:", popupDebt);
        this->labelAmount = new QLabel("Owed amount:", popupDebt);
        this->labelExp = new QLabel("Debt description:", popupDebt);
        this->labelDate = new QLabel("Due date:", popupDebt);

        tfOwedName = new QLineEdit(popupDebt);
        this->tfAmount = new QLineEdit(popupDebt);
        QIntValidator* validator = new QIntValidator(this->tfAmount);
        tfAmount->setValidator(validator);
        tfExplanation = new QLineEdit(popupDebt);
        this->cbContactName = new QComboBox(popupDebt);

        this->errLabel = new QLabel(popupDebt);
        this->errLabel->setStyleSheet("color: red;");
        errLabel->setVisible(false);

        this->dateEditDue = new QDateEdit(popupDebt);

        QString noSelection = "-";
        cbContactName->addItem(noSelection);

        //retrieve the contact names
        MainController mc;
        string userName = this->user->getUserName();
        vector<vector<QString>> contactAttributes = mc.retrieveContacts(userName);
        for(int i = 0; i < contactAttributes.size(); i++){
            cbContactName->addItem(contactAttributes.at(i).at(0));
        }

        this->btReg = new QPushButton(popupDebt);

        //set the layout management
        layout->addWidget(expLabel, 0, 0);
        layout->addWidget(labelOwedName, 1, 0);
        layout->addWidget(tfOwedName, 1, 1);
        layout->addWidget(labelAmount, 2, 0);
        layout->addWidget(tfAmount, 2, 1);
        layout->addWidget(labelExp, 3, 0);
        layout->addWidget(tfExplanation, 3, 1);
        layout->addWidget(labelDate, 4, 0);
        layout->addWidget(dateEditDue, 4, 1);
        layout->addWidget(cbContactName,1, 2);
        layout->addWidget(cbContactEnabled, 4, 2);
        layout->addWidget(errLabel, 5, 0);
        layout->addWidget(btReg, 6, 0, 1, 2);

        //set the event handling procedures
        QObject::connect(btReg, &QPushButton::clicked, this, &DebtPane::btRegSlot);

        QObject::connect(cbContactEnabled, &QCheckBox::clicked, this, &DebtPane::contactCheckSlot);
        //cbContactEnabled->setChecked(true); //after connecting checkbox signal to the lambda slot set the checked state to true initially
    }
    else{
        //The popup pane is already set and initialized, but the control nodes must be resetted to initial state
        tfOwedName->clear();
        tfAmount->clear();
        tfExplanation->clear();
        cbContactName->setCurrentIndex(0);
        this->errLabel->setVisible(false);
        //ToDo clear and reset the dateEditDue
    }




    popupDebt->show();
}
void DebtPane::btRegSlot(){
    QString cbSelection = cbContactName->currentText();
    QString noSelection = cbContactName->itemText(0);

    string givenName;
    bool selectContactEnabled = cbContactEnabled->isChecked();
    if(!selectContactEnabled){
        givenName = tfOwedName->text().toStdString();
    }
    else{
        givenName = cbSelection.toStdString();
    }

    if(!ContactsPane::isEmpty(givenName) && givenName != noSelection.toStdString()){
        MainController mc;
        string userName = this->user->getUserName();
        int debtStatus = 0; //not paid yet
        //registered holds the id of the debt in database system
        int registeredId = mc.registerDebt(userName, givenName, tfAmount->text().toInt(), tfExplanation->text().toStdString(), dateEditDue->text().toStdString(),
                                           debtStatus);
        //after registration ensure that you make a mapping between the newly layout that you will create for the debt, and the debtId for editing
        // or removal later on
        DraggableDebt* newDebt = new DraggableDebt(registeredId);
        //append to the end of our vbox
        this->vbox->addWidget(newDebt);
    }
    else{
        //display the error label with specifying name cannot be blank
        errLabel->setText("The owed name cannot be blank");
        errLabel->setVisible(true);
    }
}
void DebtPane::contactCheckSlot(bool checked){
    cbContactName->setVisible(checked);
    qDebug() << "Debug: DebtPane checkbox event handling slot has value for tfOwedName as " << tfOwedName;
    tfOwedName->setVisible(!checked);
}
DebtPane::DraggableDebt::DraggableDebt(int debtId, QWidget* parent): QWidget{parent}, debtId{debtId}{

}
void DebtPane::DraggableDebt::mousePressEvent(QMouseEvent* event){

}
void DebtPane::DraggableDebt::mouseMoveEvent(QMouseEvent* event){

}
void DebtPane::DraggableDebt::mouseReleaseEvent(QMouseEvent* event){

}
