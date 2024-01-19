#include "circularimage.h"
#include "mainscreen.h"

CircularImage::CircularImage(char initial, QWidget *parent)
    : QWidget{parent}
{
    this->initial = initial;
}
void CircularImage::paintEvent(QPaintEvent* event){
    QPainter painter(this);

    painter.setBrush(QBrush(MainScreen::USER_PP_CHAR_COLOR));
    painter.setPen(QPen(MainScreen::USER_PP_CHAR_COLOR));

    painter.drawText(5, 5, QString::fromStdString("" + this->initial));
    painter.drawArc(0, 0, CircularImage::PP_RADIUS_MID * 2, CircularImage::PP_RADIUS_MID * 2, 0, 360);
    painter.end();
}
