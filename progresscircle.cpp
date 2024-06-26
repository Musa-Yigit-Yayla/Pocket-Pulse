#include "progresscircle.h"
#include <string>

using namespace std;

ProgressCircle::ProgressCircle(QWidget *parent): ProgressCircle{0, 0, parent}{

}
ProgressCircle::ProgressCircle(int filledAmount, int totalAmount, QWidget* parent): QWidget(parent){
    this->setFixedSize(FIXED_WIDTH, FIXED_HEIGHT);
    this->setAmounts(filledAmount, totalAmount);
}
int ProgressCircle::getFilledAmount() const{
    return this->filledAmount;
}
int ProgressCircle::getTotalAmount() const{
    return this->totalAmount;
}
void ProgressCircle::setAmounts(int filledAmount, int totalAmount){
    if(totalAmount == 0){
        this->totalAmount = 1; //set it to 1 for avoiding division by zero
    }
    this->filledAmount = filledAmount;
    this->totalAmount = totalAmount;
    this->update();
}
void ProgressCircle::paintEvent(QPaintEvent* event) {
    QPainter painter(this);

    if(this->totalAmount > this->filledAmount && this->filledAmount >= 0){
        //proceed with drawing on this widget
        const int FULL_ANGLE = 300;
        int spareAngle = 360 - FULL_ANGLE;
        int sideAngle = (180 - spareAngle) / 2;
        const int STROKE_WIDTH = 6; //6 pixels

        QPen pen;
        pen.setWidth(STROKE_WIDTH);

        const QPen& defaultPen = painter.pen();

        const int MARGIN_V = 10;
        const int MARGIN_H = 10;

        double fillRatio = ((double)(this->filledAmount)) / (this->totalAmount);
        const int secondaryAngle = fillRatio * 3.0 / 2 * 240 * 16;
        QColor color = ProgressRectangle::getFillRatioRGB2(fillRatio);
        qDebug() << "Debug: color retrieved in progresscircle is rgb " << color.toRgb();
        pen.setColor(color);
        painter.setPen(pen);

        painter.setBrush(QColor::fromRgb(255, 0, 0));
        if(fillRatio >= 2 / 3.0){
            //ADJUST HERE
            painter.drawArc(MARGIN_H, MARGIN_V, this->width() - MARGIN_H, this->height() - MARGIN_V, 0, (180 + sideAngle) * 16);
            painter.drawArc(MARGIN_H, MARGIN_V, this->width() - MARGIN_H, this->height() - MARGIN_V, 0, 180 * (fillRatio - 2 / 3.0) * -16);
        }
        else{
            //painter.drawArc(MARGIN_H, MARGIN_V, this->width() - MARGIN_H, this->height() - MARGIN_V, 0 + (1 - fillRatio) * 180 * 16, (180 + sideAngle) * 16);
            //painter.drawArc(MARGIN_H, MARGIN_V, this->width() - MARGIN_H, this->height() - MARGIN_V, 0 + (fillRatio) * FULL_ANGLE * 16, 2 * sideAngle * 16);
            int startAngle = 2 * (spareAngle + sideAngle) * 16;
            painter.drawArc(MARGIN_H, MARGIN_V, this->width() - MARGIN_H, this->height() - MARGIN_V, startAngle, -1 * secondaryAngle);
        }


        //draw the correlated text in the middle
        painter.setPen(defaultPen);
        painter.setBrush(QColor::fromRgb(0, 0, 0));
        string text = to_string(this->filledAmount) + "/" + to_string(this->totalAmount);
        painter.drawText(this->width() / 2, this->height() / 2, QString::fromStdString(text));
    }
    event->ignore();
}
