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

    this->gridPane->addLayout(this->financialGoalsGrid, 0, 0);
    this->gridPane->addLayout(this->rectGrid, 1, 0); //change the indexing later on
    this->gridPane->addLayout(this->transactionsGrid, 1, 1);
    qDebug() << "Debug: FingoalPane constructor is about to return to the caller";

}
void FingoalPane::setFinancialGoalsGrid(){
    QLabel* goalIconLabel = new QLabel(this);
    QString imgPath = QString::fromStdString(MainScreen::ICONS_FOLDER_PATH + "\\icons2.png");
    QImage img(imgPath);
    QPixmap pixmap = QPixmap::fromImage(img);
    goalIconLabel->setPixmap(pixmap);

    //add the labels into the hbox for financial goals' header
    this->hboxGoalHeader->addWidget(goalIconLabel);
    this->hboxGoalHeader->addWidget(this->labelGoal);
    this->financialGoalsGrid->addLayout(this->hboxGoalHeader, 0, 0);
    //Proceed
}
void FingoalPane::setRectGrid(vector<int>& spenditureGoals){
    //If the given spenditureGoals vector is empty, we should construct one containing all 0s
    if(spenditureGoals.size() == 0){
        for(int i = 0; i < this->RECTS_LENGTH; i++){
            spenditureGoals.push_back(i);
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

            int currSum = bc.sumSentTransactions(userId, i, month, year);
            int currGoal = spenditureGoals.at(i);
            if(currSum == 0){
                currFillRatio = 0;
            }
            else{
                currFillRatio = currGoal / currSum;
            }
            labelTopStr = QString::fromStdString(to_string(currSum) + "/" + to_string(currSum));
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
                currFillRatio = transactionsSum / spenditureGoalsSum;
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
    this->transactionSA->setLayout(this->transactionsVBox);
    this->transactionSA->setWidgetResizable(true);
    this->transactionsGrid->addWidget(this->transactionSA, 1, 0);


    QObject::connect(this->cbTransactionCategory, &QComboBox::currentIndexChanged, this, &FingoalPane::cbTransactionSlot);
    //since initial selection is All, display each and every transaction in the given vector sorted by their dates
    this->cbTransactionCategory->setCurrentIndex(0);
}
/*FingoalPane::~FingoalPane(){
    //deallocate dynamically allocated rectangles and the container array
    for(int i = 0; i < this->RECTS_LENGTH; i++){
        delete this->spenditureRects[i];
    }
    delete[] this->spenditureRects;
}*/
void FingoalPane::refreshFinancialGoals(){

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
        QLayoutItem* item = this->transactionsVBox->takeAt(0);
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
const vector<const QString*> FingoalPane::CATEGORY_NAMES = {new QString("Health"), new QString("Education"), new QString("Market/Grocery"),
                                                             new QString("Entertainment"), new QString("Vehicle/Fuel"), new QString("Fees"),
                                                             new QString("Other")};
