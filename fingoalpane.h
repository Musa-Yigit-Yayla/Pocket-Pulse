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
#include <cstddef>
#include <vector>
#include "abstractpane.h"
#include "user.h"
#include "progressrectangle.h"

#ifndef FINGOALPANE_H
#define FINGOALPANE_H


class FingoalPane: public AbstractPane{
Q_OBJECT
private:
    QGridLayout* rectGrid = new QGridLayout(this); //gridpane to contain the rectangular spenditure views
    QGridLayout* transactionsGrid = new QGridLayout(this); //gridPane to contain sent transactions for this month
    QScrollArea* transactionSA = new QScrollArea(this);
    QVBoxLayout* transactionsVBox = new QVBoxLayout(this);
    QComboBox* cbTransactionCategory = new QComboBox(this);


    const int RECTS_LENGTH = 8;
    ProgressRectangle** spenditureRects = new ProgressRectangle*[RECTS_LENGTH];

    const int RECT_GRID_WIDTH = 450;
    const int RECT_HEIGHT = 180;
    const int RECT_WIDTH = 45;
public:
    FingoalPane(User* user, QWidget* parent = nullptr);
    //~FingoalPane();
    void redrawRectangles();

    static void paintProgressRect(QRect& rect, double successRatio);
private:
    void setRectGrid(vector<int>& spenditureGoals);
    void setTransactionsGrid(vector<vector<QVariant>>& transactions);
public slots:
    void cbTransactionSlot(int index);

};

#endif // FINGOALPANE_H
