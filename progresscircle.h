#ifndef PROGRESSCIRCLE_H
#define PROGRESSCIRCLE_H

#include <QWidget>
#include <QPaintEvent>
#include <QPainter>
#include <QPen>
#include "progressrectangle.h"

//!!!YOU CAN USE STACKPANE FOR IMPLEMENTING THIS WIDGET

class ProgressCircle : public QWidget{
Q_OBJECT
private:
    int filledAmount;
    int totalAmount;
public:
    ProgressCircle(QWidget* parent = nullptr);
    ProgressCircle(int filledAmount, int totalAmount, QWidget *parent = nullptr);
    int getFilledAmount() const;
    int getTotalAmount() const;
    void setAmounts(int filledAmount, int totalAmount); //triggers a redrawal of the circle
    void paintEvent(QPaintEvent* event) override;

    static const int FIXED_HEIGHT = 200;
    static const int FIXED_WIDTH = 250;
signals:

};

#endif // PROGRESSCIRCLE_H
