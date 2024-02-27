#include "debtpane.h"
#include "contactspane.h"
#include "maincontroller.h"
#include "bankingcontroller.h"
#include <string>

using namespace std;

DebtPane::DebtPane(User* user, QWidget* parent): AbstractPane{user, parent}{

    QObject::connect(this->btAddDebt, &QPushButton::clicked, this, &DebtPane::addDebtSlot);
}

void DebtPane::addDebtSlot(){
    QWidget* popupDebt = new QWidget();

    QLineEdit* tfOwedName = new QLineEdit(popupDebt);
    QLineEdit* tfAmount = new QLineEdit(popupDebt);
    QIntValidator* validator = new QIntValidator(tfAmount);
    tfAmount->setValidator(validator);
    QLineEdit* tfExplanation = new QLineEdit(popupDebt);
    QComboBox* cbContactName = new QComboBox(popupDebt);
    QLabel* errLabel = new QLabel(popupDebt);
    errLabel->setVisible(false);
    QDateEdit* dateEditDue = new QDateEdit(popupDebt);
    QString noSelection = "-";
    cbContactName->addItem(noSelection);

    //retrieve the contact names
    MainController mc;
    string userName = this->user->getUserName();
    vector<vector<QString>> contactAttributes = mc.retrieveContacts(userName);
    for(int i = 0; i < contactAttributes.size(); i++){
        cbContactName->addItem(contactAttributes.at(i).at(0));
    }

    QPushButton* btReg = new QPushButton(popupDebt);
    QObject::connect(btReg, &QPushButton::clicked, [&](){
        QString cbSelection = cbContactName->currentText();

        string givenName;
        if(cbSelection == noSelection){
            givenName = tfOwedName->text().toStdString();
        }
        else{
            givenName = cbSelection.toStdString();
        }

        if(!ContactsPane::isEmpty(givenName)){
            int debtStatus = 0; //not paid yet
            //registered holds the id of the debt in database system
            int registeredId = mc.registerDebt(userName, givenName, tfAmount->text().toInt(), tfExplanation->text().toStdString(), dateEditDue->text().toStdString(),
                                                debtStatus);
            //after registration ensure that you make a mapping between the newly layout that you will create for the debt, and the debtId for editing
            // or removal later on
            DraggableDebt* newDebt = new DraggableDebt(registeredId, this);
            //append to the end of our vbox
            this->vbox->addWidget(newDebt);
        }
        else{
            //display the error label with specifying name cannot be blank
            errLabel->setText("The owed name cannot be blank");
            errLabel->setVisible(true);
        }

    });


    popupDebt->show();
}
DebtPane::DraggableDebt::DraggableDebt(int debtId, QWidget* parent): QWidget{parent}, debtId{debtId}{

}
void DebtPane::DraggableDebt::mousePressEvent(QMouseEvent* event){

}
void DebtPane::DraggableDebt::mouseMoveEvent(QMouseEvent* event){

}
void DebtPane::DraggableDebt::mouseReleaseEvent(QMouseEvent* event){

}
