#include "fingoalpane.h"
#include "maincontroller.h"
#include "mainscreen.h"
#include "bankingcontroller.h"

FingoalPane::FingoalPane(User* user, QWidget* parent): AbstractPane{user, parent}{
    qDebug() << "Debug: FingoalPane constructor scope has been entered.";
    //retrieve the date of today
    QDate currTime = QDateTime::currentDateTime().date();
    int month = currTime.month();
    int year = currTime.year();

    //retrieve the financial goals registered for this month
    MainController mc;
    BankingController bc;
    vector<int> spenditureGoals = mc.getUserMonthlyGoals(this->user->getUserName(), month, year);
    //int userId = mc.getUserId(this->user->getUserName());
    //vector<vector<QVariant>> transactions = bc.getSpentTransactions(userId, month, year); //all columns are returned

    this->setTransactionsGrid();
    this->setRectGrid(spenditureGoals);
    this->setFinancialGoalsGrid();


    this->gridPane->addLayout(this->rectGrid, 0, 0, 2, 1); //change the indexing later on
    this->gridPane->addLayout(this->financialGoalsGrid, 0, 1);
    this->gridPane->addLayout(this->transactionsGrid, 1, 1);
    qDebug() << "Debug: FingoalPane constructor is about to return to the caller";

}
void FingoalPane::setFinancialGoalsGrid(){
    QLabel* goalIconLabel = new QLabel(this);
    QString imgPath = QString::fromStdString(MainScreen::ICONS_FOLDER_PATH + "\\icon2.png");
    QImage img(imgPath);
    QPixmap pixmap = QPixmap::fromImage(img);
    pixmap = pixmap.scaled(25, 25);
    goalIconLabel->setPixmap(pixmap);

    this->expTextArea->setFixedWidth(200);
    //add the textarea and reg button to the correlated vbox
    this->registerFinGoalPane->addWidget(this->expTextArea);
    this->registerFinGoalPane->addWidget(this->btRegFinGoal);

    const int VBOX_GOALS_SPACING = 20;
    this->vboxGoals->setSpacing(VBOX_GOALS_SPACING);
    this->refreshFinancialGoals();

    //wrap the vboxGoals in a scroll area after you have wrapped it into an intermediate widget
    QWidget* vboxGoalWrapper = new QWidget(this);
    QScrollArea* vboxSA = new QScrollArea(this);
    vboxGoalWrapper->setLayout(this->vboxGoals);
    vboxSA->setWidget(vboxGoalWrapper);



    //add the labels into the hbox for financial goals' header
    this->hboxGoalHeader->addWidget(goalIconLabel);
    this->hboxGoalHeader->addWidget(this->labelGoal);
    this->financialGoalsGrid->addLayout(this->hboxGoalHeader, 0, 0);
    //DONT FORGET TO ADD THE PROGRESS CIRCLE INTO THIS LINE IN POSITION 1, 0
    this->financialGoalsGrid->addLayout(this->registerFinGoalPane, 2, 0);
    this->financialGoalsGrid->addWidget(vboxSA, 0, 1, 3, 1);


    QObject::connect(this->btRegFinGoal, &QPushButton::clicked, this, &FingoalPane::regFinGoalSlot);
}
void FingoalPane::setRectGrid(vector<int>& spenditureGoals){
    //If the given spenditureGoals vector is empty, we should construct one containing all 0s
    if(spenditureGoals.size() == 0){
        for(int i = 0; i < this->RECTS_LENGTH; i++){
            spenditureGoals.push_back(0);
        }
    }
    QDate currTime = QDateTime::currentDateTime().date();
    int month = currTime.month();
    int year = currTime.year();

    //allocate the rectangles and fill the rect array
    BankingController bc;
    MainController mc;

    int userId = mc.getUserId(this->user->getUserName());

    int spenditureGoalsSum = 0;
    int transactionsSum = 0;
    for(int i = 0; i < this->RECTS_LENGTH; i++){
        double currFillRatio;
        ProgressRectangle* currRect = nullptr;
        QString labelTopStr;
        QString labelBottomStr;
        if(i != 7){ //the regular case of calculating transactions wrt their category

            int currSum = bc.sumSentTransactions(userId, i + 1, month, year); // i + 1 to match the database category attribute values directly
            int currGoal = spenditureGoals.at(i);

            qDebug() << "Debug: currSum and currGoal are respectively " << currSum << ", and " << currGoal << " in FingoalPane::setRectGrid";
            if(currSum == 0){
                currFillRatio = 0;
            }
            else{
                currFillRatio = currSum / (double)(currGoal);
            }
            qDebug() << "Debug: currFillRatio in FingoalPane::setRectGrid is " << currFillRatio;
            labelTopStr = QString::fromStdString(to_string(currSum) + "/" + to_string(currGoal));
            labelBottomStr = *FingoalPane::CATEGORY_NAMES.at(i);
            currRect = new ProgressRectangle(RECT_WIDTH, RECT_HEIGHT, currFillRatio, this);
            transactionsSum += currSum;
            spenditureGoalsSum += currGoal;
        }
        else{
            //the special case where we instantiate the total sum rectangle
            if(spenditureGoalsSum == 0){
                currFillRatio = 0;
            }
            else{
                currFillRatio = transactionsSum / (double)spenditureGoalsSum;
            }
            labelTopStr = QString::fromStdString(to_string(transactionsSum) + "/" + to_string(spenditureGoalsSum));
            labelBottomStr = "Total";
            currRect = new ProgressRectangle(RECT_WIDTH, RECT_HEIGHT, currFillRatio, this);
        }
        this->spenditureRects[i] = currRect;
        //wrap the curr progress rectangle into the widget and add it to the transactions grid

        QWidget* wrapper = new QWidget(this); //widget which will have our currRect wrapped
        QVBoxLayout* currVBox = new QVBoxLayout(wrapper);
        QLabel* labelTop = new QLabel(labelTopStr, wrapper);
        QLabel* labelBottom = new QLabel(labelBottomStr, wrapper);

        currVBox->addWidget(labelTop);
        currVBox->addWidget(currRect);
        currVBox->addWidget(labelBottom);
        wrapper->setLayout(currVBox);

        int rowIndex = i / 4, columnIndex = i % 4;
        this->rectGrid->addWidget(wrapper, rowIndex, columnIndex);
    }
}
void FingoalPane::setTransactionsGrid(){
    //initialize the contents of the combobox
    QList<QString> categories = {"All", "Health", "Education", "Market/Grocery", "Entertainment", "Vehicle & Petrol", "Fees", "Other"}; //all specifies select all transactions
    this->cbTransactionCategory->addItems(QStringList::fromVector(categories));

    QLabel* expLabel = new QLabel("Sent Transactions", this);
    expLabel->setStyleSheet("font-size: 16px;");
    this->transactionsGrid->addWidget(expLabel, 0, 0);
    this->transactionsGrid->addWidget(this->cbTransactionCategory, 0, 1);

    //set the scroll area
    const int TRANSACTION_SA_MINHEIGHT = 350;
    this->transactionSA->setFixedHeight(TRANSACTION_SA_MINHEIGHT);
    this->transactionSA->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    this->transactionSA->setWidgetResizable(true);
    QWidget* wrapper = new QWidget(this);
    wrapper->setLayout(this->transactionsVBox);
    this->transactionSA->setWidget(wrapper);

    this->transactionsGrid->addWidget(this->transactionSA, 1, 0);


    QObject::connect(this->cbTransactionCategory, &QComboBox::currentIndexChanged, this, &FingoalPane::cbTransactionSlot);
    //since initial selection is All, display each and every transaction in the given vector sorted by their dates
    this->cbTransactionSlot(0);
}
/*FingoalPane::~FingoalPane(){
    //deallocate dynamically allocated rectangles and the container array
    for(int i = 0; i < this->RECTS_LENGTH; i++){
        delete this->spenditureRects[i];
    }
    delete[] this->spenditureRects;
}*/
void FingoalPane::refreshFinancialGoals(){
    //clear out the current content of the vboxGoals
    while(this->vboxGoals->count() > 0){
        QLayout* currHBox =  this->vboxGoals->takeAt(0)->layout();

        //delete child widgets
        delete currHBox->itemAt(0)->widget();
        delete currHBox->itemAt(1)->widget();

        delete currHBox;
    }

    //clear the current button id map
    this->fingoalMap.clear();

    //fetch existing financial goals (ones that are not reached yet) and add them into the registerFinGoalPane
    MainController mc;
    unordered_map<int, string> goals = mc.retrieveFinancialGoals(this->user->getUserName(), false); // goals along with fingoal ids
    for(unordered_map<int, string>::iterator it = goals.begin(); it != goals.end(); it++){
        string currGoal = it->second;
        QLabel* goalLabel = new QLabel(QString::fromStdString("● " + currGoal), this);
        goalLabel->setStyleSheet("border-color: green;");

        QToolButton* btMarkDone = new QToolButton(this);
        btMarkDone->setText("Mark as\n achieved");

        int goalID = it->first;
        this->fingoalMap.insert(make_pair(goalID, btMarkDone));

        QHBoxLayout* hbox = new QHBoxLayout(this);
        hbox->addWidget(goalLabel);
        hbox->addWidget(btMarkDone);
        this->vboxGoals->addLayout(hbox);
        QObject::connect(btMarkDone, &QToolButton::clicked, this, &FingoalPane::redrawRectangles);
    }
}
void FingoalPane::redrawRectangles(){
    //rectangles are already instantiated, hence just redraw by changing the fill ratios


}

//Draw the given rectangle to display progress of the attribıute represented by it
//successRatio must be a floating point number [0, 1]. 0 indicates no progress (full green), 1 indicates full capacity (full red)
void FingoalPane::paintProgressRect(QRect& rect, double successRatio){

}
void FingoalPane::cbTransactionSlot(int index){
    QDate currTime = QDateTime::currentDateTime().date();
    int month = currTime.month();
    int year = currTime.year();

    MainController mc;
    BankingController bc;

    int userId = mc.getUserId(this->user->getUserName());
    vector<vector<QVariant>> transactions = bc.getSpentTransactions(userId, month, year, index); //index stands for the category
    qDebug() << "Debug: FingoalPane::cbTransactionSlot transactions.size() yields " << transactions.size();

    //remove the current content of the vbox
    while(this->transactionsVBox->count() > 0){
        qDebug() << "Debug: Deleting layoutItem in cbTransactionSlot, and current vbox->count() is " << transactionsVBox->count();
        QLayoutItem* item = this->transactionsVBox->takeAt(0);
        QLayout* childLayout = item->layout();
        //delete the child widgets of the layout
        while(item->layout()->count() > 0){
            QWidget* currWidget = childLayout->takeAt(0)->widget();
            delete currWidget;
        }
        delete item;
    }

    for(vector<QVariant> currTransaction: transactions){
        //recall that transaction vector holds all columns, use the ones you need
        QHBoxLayout* hbox = new QHBoxLayout();

        int receiverId = currTransaction.at(2).toInt();
        QLabel* sentNameLabel = new QLabel(QString::fromStdString(bc.getAccountAttribute(receiverId, BankingController::ACCOUNT_ATTRIBUTES::FIRST_NAME) + " " +
                                                            bc.getAccountAttribute(receiverId, BankingController::ACCOUNT_ATTRIBUTES::LAST_NAME)));
        QLabel* amountLabel = new QLabel(currTransaction.at(3).toString());
        int category = currTransaction.at(4).toInt();
        QLabel* categoryLabel = new QLabel(this->cbTransactionCategory->itemText(category));
        QLabel* dateLabel = new QLabel(currTransaction.at(5).toString());

        hbox->addWidget(sentNameLabel);
        hbox->addWidget(amountLabel);
        hbox->addWidget(categoryLabel);
        hbox->addWidget(dateLabel);

        this->transactionsVBox->addLayout(hbox);
    }
}
void FingoalPane::regFinGoalSlot(){
    //register a new financial goal if the textarea is not empty
    string goalStr = this->expTextArea->toPlainText().toStdString();
    qDebug() << "Debug: FingoalPane::regFinGoalSlot invoked with goalStr " << goalStr;
    if(goalStr != ""){
        QDate currDate = QDateTime::currentDateTime().date();
        int month = currDate.month();
        int day = currDate.day();
        int year = currDate.year();

        string dateStr = to_string(month) + "/" + to_string(day) + "/" + to_string(year);

        MainController mc;
        mc.registerFinancialGoal(this->user->getUserName(), goalStr, dateStr);
        this->refreshFinancialGoals();
        this->expTextArea->clear();
    }
}
void FingoalPane::markFinGoalComplete(){

}
const vector<const QString*> FingoalPane::CATEGORY_NAMES = {new QString("Health"), new QString("Education"), new QString("Market/Grocery"),
                                                             new QString("Entertainment"), new QString("Vehicle/Fuel"), new QString("Fees"),
                                                             new QString("Other")};
