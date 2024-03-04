#include <QObject>
#include <QGridLayout>
#include <QLabel>
#include <QRect>
#include <cstddef>
#include "abstractpane.h"
#include "user.h"
#include "progressrectangle.h"

#ifndef FINGOALPANE_H
#define FINGOALPANE_H


class FingoalPane: public AbstractPane{
Q_OBJECT
private:
    QGridLayout* rectGrid = new QGridLayout(this); //gridpane to contain the rectangular spenditure views
    const int RECTS_LENGTH = 8;
    ProgressRectangle** spenditureRects = new ProgressRectangle*[RECTS_LENGTH];

    const int RECT_GRID_WIDTH = 450;
    const int RECT_HEIGHT = 180;
    const int RECT_WIDTH = 45;
public:
    FingoalPane(User* user, QWidget* parent = nullptr);
    ~FingoalPane();
    void redrawRectangles();

    static void paintProgressRect(QRect& rect, double successRatio);

};

#endif // FINGOALPANE_H
