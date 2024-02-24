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

        if(cbSelection == noSelection){
            //use the name specified in the textfield and verify that it is not empty
            string givenName = tfOwedName->text().toStdString();
            if(!ContactsPane::isEmpty(givenName)){
                int debtStatus = 0; //not paid yet
                bool registered = mc.registerDebt(userName, givenName, tfAmount->text().toInt(), tfExplanation->text().toStdString(), dateEditDue->text().toStdString(),
                                                    debtStatus);
                //after registration ensure that you make a mapping between the newly layout that you will create for the debt, and the debtId for editing
                // or removal later on

            }
            else{
                //display the error label with specifying name cannot be blank
                errLabel->setText("The owed name cannot be blank");
                errLabel->setVisible(true);
            }
        }
        else{

        }
    });


    popupDebt->show();
}
