#include "progresscircle.h"


ProgressCircle::ProgressCircle(QWidget *parent): ProgressCircle{0, 0, parent}{

}
ProgressCircle::ProgressCircle(int filledAmount, int totalAmount, QWidget* parent): QWidget(parent){
    this->filledAmount = filledAmount;
    this->totalAmount = totalAmount;
    this->setFixedSize(FIXED_WIDTH, FIXED_HEIGHT);
}
int ProgressCircle::getFilledAmount() const{
    return this->filledAmount;
}
int ProgressCircle::getTotalAmount() const{
    return this->totalAmount;
}
void ProgressCircle::setAmounts(int filledAmount, int totalAmount){
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
        const int STROKE_WIDTH = 6; //4 pixels

        QPen pen;
        pen.setWidth(STROKE_WIDTH);

        double fillRatio = ((double)(this->filledAmount)) / (this->totalAmount);
        QColor color = ProgressRectangle::getFillRatioRGB(fillRatio);
        painter.setPen(pen);
        painter.setBrush(color);
        painter.drawArc(0, 0, this->width(), this->height(), 0, 180 + sideAngle);
        painter.drawArc(0, 0, this->width(), this->height(), 0, 0 - spareAngle);
        //painter.drawText() PROCEED
    }
    event->ignore();
}
