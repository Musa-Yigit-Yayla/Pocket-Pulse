#include <QObject>
#include <QGridLayout>
#include <QLabel>
#include <QRect>
#include <QDateTime>
#include <QScrollArea>
#include <QComboBox>
#include <QList>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QPushButton>
#include <QToolButton>
#include <cstddef>
#include <vector>
#include <unordered_map>
#include "abstractpane.h"
#include "user.h"
#include "progressrectangle.h"
#include "progresscircle.h"

#ifndef FINGOALPANE_H
#define FINGOALPANE_H


class FingoalPane: public AbstractPane{
Q_OBJECT
private:
    QGridLayout* gridPane = new QGridLayout(this); //main grid pane

    QGridLayout* rectGrid = new QGridLayout(this); //gridpane to contain the rectangular spenditure views
    QGridLayout* transactionsGrid = new QGridLayout(this); //gridPane to contain sent transactions for this month
    QScrollArea* transactionSA = new QScrollArea(this);
    QVBoxLayout* transactionsVBox = new QVBoxLayout(this->transactionSA);
    QComboBox* cbTransactionCategory = new QComboBox(this);

    QGridLayout* financialGoalsGrid = new QGridLayout(this);
    QHBoxLayout* hboxGoalHeader = new QHBoxLayout(this);
    QLabel* labelGoal = new QLabel("Financial Goals", this);
    unordered_map<QToolButton*, int> fingoalMap;

    QVBoxLayout* vboxGoals = new QVBoxLayout(this); //vbox to wrap financial goals related stuff
    QHBoxLayout* hboxFinReg = new QHBoxLayout(this); // will be used to wrap the circular widget (showing goal finish success) and register fingoal pane
    ProgressCircle* progressCircle = nullptr;
    QVBoxLayout* registerFinGoalPane = new QVBoxLayout(this);
    QTextEdit* expTextArea = new QTextEdit(this);
    QPushButton* btRegFinGoal = new QPushButton("Add financial\n    goal", this);


    const int RECTS_LENGTH = 8;
    ProgressRectangle** spenditureRects = new ProgressRectangle*[RECTS_LENGTH];

    const int RECT_GRID_WIDTH = 450;
    const int RECT_HEIGHT = 180;
    const int RECT_WIDTH = 45;
public:
    FingoalPane(User* user, QWidget* parent = nullptr);
    //~FingoalPane();
    void redrawRectangles();
    void refreshFinancialGoals();

    static void paintProgressRect(QRect& rect, double successRatio);
    static const vector<const QString*> CATEGORY_NAMES;
private:
    void setRectGrid(vector<int>& spenditureGoals);
    void setTransactionsGrid();
    void setFinancialGoalsGrid();
public slots:
    void cbTransactionSlot(int index);
    void regFinGoalSlot();
    void markFinGoalComplete();
};
#endif // FINGOALPANE_H
