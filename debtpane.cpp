#include "debtpane.h"
#include "contactspane.h"
#include "maincontroller.h"
#include "bankingcontroller.h"
#include <string>

using namespace std;

DebtPane::DebtPane(User* user, QWidget* parent): AbstractPane{user, parent}{

    //initialize debtpane layout management and child widgets
    this->initializeDebtPane();
    this->sa->setWidgetResizable(true);
    this->sa->setLayout(this->vbox);
    this->gridPane->addWidget(this->sa, 0, 0);

    const int btAddDebtLength = 200;
    this->btAddDebt->setMaximumWidth(btAddDebtLength);
    QSpacerItem* spacer = new QSpacerItem(150, 100, QSizePolicy::Preferred, QSizePolicy::Preferred);
    QHBoxLayout* hboxBtWrapper = new QHBoxLayout(this);
    hboxBtWrapper->addSpacerItem(spacer);
    hboxBtWrapper->addWidget(btAddDebt);

    //this->gridPane->addWidget(this->btAddDebt, 1, 0);
    this->gridPane->addLayout(hboxBtWrapper, 1, 0);


    QObject::connect(this->btAddDebt, &QPushButton::clicked, this, &DebtPane::addDebtSlot);
}
QVBoxLayout*& DebtPane::getVBox(){
    return this->vbox;
}
//private method to initialize the contents of the debts if any exist
void DebtPane::initializeDebtPane(){
    //retrieve the debts ordered
    MainController mc;

    vector<vector<QVariant>> existingDebts = mc.getAllDebts(this->user->getUserName(), true);
    for(vector<QVariant> debt: existingDebts){
        int currDebtId = debt.at(0).toInt();
        int currDebtPriority = debt.at(7).toInt();
        QString owedName = debt.at(2).toString();
        QString owedAmount = debt.at(3).toString();
        QString owedExp = debt.at(4).toString();
        QString owedDate = debt.at(5).toString();
        DraggableDebt* currDebt = new DraggableDebt(currDebtId, currDebtPriority, owedName, owedAmount, owedExp, owedDate, this);
        this->vbox->addWidget(currDebt);
    }


}
//Method to be invoked when priority of a draggable debt is altered
void DebtPane::refreshVBox(){
    //sort the current draggableDebt instances with respect to their priority (in ascending order) after storing them in a vector with improved bubble sort
    vector<DraggableDebt*> children;

    for(int i = 0; i < this->vbox->count(); i++){
        DraggableDebt* currDebt = qobject_cast<DraggableDebt*>(this->vbox->itemAt(i)->widget());
        if(currDebt != NULL){
            children.push_back(currDebt);
        }
    }
    for(int i = 0; i < children.size() - 1; i++){
        bool swapped = false;
        for(int j = 0; j < children.size() - i - 1; j++){
            DraggableDebt* curr = children.at(j);
            DraggableDebt* next = children.at(j + 1);
            if(next->getPriority() < curr->getPriority()){
                children.at(j) = next;
                children.at(j + 1) = curr;
                swapped = true;
            }
        }
        if(!swapped){
            //vector is sorted apparently
            break;
        }
    }
    //clear all elements of the vbox
    while(vbox->count() > 0){
        this->vbox->removeItem(this->vbox->itemAt(0));
    }
    //reconstruct the vbox by adding widgets back in
    for(DraggableDebt* debt: children){
        this->vbox->addWidget(debt);
        debt->resetInitialPosSet();
    }
}
inline void DebtPane::setContactComboBox(){
    //clear the combobox contents
    this->cbContactName->clear();

    QString noSelection = "-";
    cbContactName->addItem(noSelection);

    //retrieve the contact names
    MainController mc;
    string userName = this->user->getUserName();
    vector<vector<QString>> contactAttributes = mc.retrieveContacts(userName);
    for(int i = 0; i < contactAttributes.size(); i++){
        cbContactName->addItem(contactAttributes.at(i).at(0));
    }
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


        this->setContactComboBox();

        this->btReg = new QPushButton("Add the debt", popupDebt);
        this->cbContactEnabled->setChecked(true);

        //set the layout management
        layout->addWidget(expLabel, 0, 0);
        layout->addWidget(labelOwedName, 1, 0);
        //layout->addWidget(tfOwedName, 1, 1);
        layout->addWidget(labelAmount, 2, 0);
        layout->addWidget(tfAmount, 2, 1);
        layout->addWidget(labelExp, 3, 0);
        layout->addWidget(tfExplanation, 3, 1);
        layout->addWidget(labelDate, 4, 0);
        layout->addWidget(dateEditDue, 4, 1);
        layout->addWidget(cbContactName,1, 1);
        layout->addWidget(cbContactEnabled, 4, 2);
        layout->addWidget(errLabel, 5, 0);
        layout->addWidget(btReg, 6, 0, 1, 2);

        this->tfOwedName->setVisible(false);
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

        //Update the contact name combobox since the user may have added new contacts
        this->setContactComboBox();
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
        int debtPriority = mc.getDebtPriority(registeredId);
        QString owedName = QString::fromStdString(givenName);
        QString owedAmount = tfAmount->text();
        QString debtExp = tfExplanation->text();
        QString debtDue = dateEditDue->text();
        DraggableDebt* newDebt = new DraggableDebt(registeredId, debtPriority, owedName, owedAmount, debtExp, debtDue, this);
        //append to the end of our vbox
        this->vbox->addWidget(newDebt);
        //set the debt's initial pos

        //close the popupdebt
        this->popupDebt->close();

    }
    else{
        //display the error label with specifying name cannot be blank
        errLabel->setText("The owed name cannot be blank");
        errLabel->setVisible(true);
    }
}
void DebtPane::contactCheckSlot(bool checked){
    cbContactName->setVisible(checked);
    //qDebug() << "Debug: DebtPane checkbox event handling slot has value for tfOwedName as " << tfOwedName;
    tfOwedName->setVisible(!checked);
    int row = 1, column = 1;
    QLayoutItem* item = this->layout->itemAtPosition(1, 1);
    this->layout->removeItem(item);
    QWidget* selectedWidget = nullptr;
    if(checked){
        selectedWidget = this->cbContactName;
    }
    else{
        selectedWidget = this->tfOwedName;

    }
    this->layout->addWidget(selectedWidget, row, column);
}
DraggableDebt::DraggableDebt(int debtId, int debtPriority, QString& owedName, QString& amount, QString& explanation, QString& date, QWidget* parent): QWidget{parent}, debtId{debtId}{
    //retrieve desired debt attributes of the tuple with given debtId
    this->debtId = debtId;
    this->debtPriority = debtPriority;
    this->btMarkPaid->setText("Mark as paid");
    this->btMarkPaid->setStyleSheet("color: rgb(15, 250, 5");

    this->setMaximumSize(MAX_WIDTH, MAX_HEIGHT);

    //append $ onto amount str
    amount.append("$");

    this->labelName = new QLabel(owedName, this);
    this->labelAmount = new QLabel(amount, this);
    this->labelExp = new QLabel(explanation, this);
    this->labelDate = new QLabel(date, this);

    this->hbox->addWidget(this->labelName);
    this->hbox->addWidget(this->labelAmount);
    this->hbox->addWidget(this->labelExp);
    this->hbox->addWidget(this->labelDate);
    this->hbox->addWidget(this->btMarkPaid);

    //set hbox color
    this->setStyleSheet("color: rgb(180, 80, 6); border: 2px solid black;");
    QObject::connect(this->btMarkPaid, &QToolButton::clicked, this, &DraggableDebt::markAsPaidSlot);
}
int DraggableDebt::getPriority() const{
    return this->debtPriority;
}
void DraggableDebt::setPriority(int debtPriority){
    this->debtPriority = debtPriority;
    //Update the value persisted in the database as well
    MainController mc;
    mc.setDebtPriority(this->debtId, this->debtPriority);
}
void DraggableDebt::setInitialPos(QPoint point){
    qDebug() << "Debug: DraggableDebt::setInitialPos received point " << point;
    this->initialPos = point;
}
//Invoked from refreshVBox of debtpane
void DraggableDebt::resetInitialPosSet(){
    this->initialPosSet = false;
}
void DraggableDebt::mousePressEvent(QMouseEvent* event){
    qDebug() << "Debug: DraggableDebt mousePressEvent handler has been invoked. this->pos() yields " << this->pos() << " mapFromGlobal yields " << this->mapFromGlobal(this->pos());
    //set the initial pos
    if(!this->initialPosSet){
        this->initialPos = this->pos();
        this->initialPosSet = true;
    }


    //invoke mouseMoveEvent
    this->mouseMoveEvent(event);
}
void DraggableDebt::mouseMoveEvent(QMouseEvent* event){
    QPoint pos = event->globalPos();

    //event pos represents the centered position hence subtract the offset
    pos = pos - MOVE_OFFSET;

    //qDebug() << "Debug: DraggableDebt mouseMoveEvent handler has been invoked with pos " << pos;
    //qDebug() << "Debug: current pos with offset applied yields " << pos - MOVE_OFFSET;

    if(event->buttons() & Qt::LeftButton){ //unary and with leftbutton to check whether left button is pressed (among possible other buttons)
        this->move(pos);
        event->accept(); //prevent the event from further handling
    }
    else{
        event->ignore();
    }

}
void DraggableDebt::mouseReleaseEvent(QMouseEvent* event){
    qDebug() << "Debug: DraggableDebt mouseReleaseEvent handler has been invoked";
    qDebug() << "Debug: DraggableWidget's parent's parent widget pointer is " << this->parent()->parent();
    qDebug() << "Debug: DraggableDebt's initial pos is " << this->initialPos;

    //Upon mouse release we should check whether we are in the boundaries of another DraggableDebt instance. If so we update priorities. Else we set back to initial position

    bool updated = false;
    //Iterate over all DraggableDebt instances persisted within vbox of debtpane
    DebtPane* debtPane = qobject_cast<DebtPane*>(this->parent()->parent()); //direct parent of the DraggableDebts is QScrollArea, hence retrieve its parent
    if(debtPane != NULL){
        QVBoxLayout* vbox = debtPane->getVBox();
        for(int i = 0; i < vbox->count(); i++){
            DraggableDebt* currDebt = qobject_cast<DraggableDebt*>(vbox->itemAt(i)->widget());
            if(currDebt != NULL && currDebt != this){
                //check whether this debt instance contains the event position
                QPoint eventPoint = event->globalPosition().toPoint();
                if(currDebt->geometry().contains(eventPoint)){
                    //swap this debt with currDebt
                    int tempPriority = this->debtPriority;
                    this->setPriority(currDebt->debtPriority);
                    currDebt->setPriority(tempPriority);

                    //refresh the debtPane's vbox
                    debtPane->refreshVBox();
                    updated = true;
                    break;
                }
            }
        }
    }
    qDebug() << "Debug: DraggableDebt::mouseReleaseEvent updated status is " << updated;
    if(!updated){
        //return back to initial pos
        this->move(this->initialPos);
    }

}
void DraggableDebt::markAsPaidSlot(){
    //remove the widget from the vbox
    DebtPane* debtPane = qobject_cast<DebtPane*>(this->parent()->parent());
    if(debtPane != NULL){

        QVBoxLayout* debtVBox = debtPane->getVBox();
        debtVBox->removeWidget(this);
        //update this debt's paid satus in the database
        MainController mc;
        mc.markDebtAsPaid(this->debtId);
        //delete this object now as we do not need it visually anymore
        delete this;
    }
}

const QPoint DraggableDebt::MOVE_OFFSET(MAX_WIDTH / 2, MAX_HEIGHT / 2);
