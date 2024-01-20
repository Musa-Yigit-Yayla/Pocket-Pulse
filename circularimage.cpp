#include "circularimage.h"
#include "mainscreen.h"
#include <qDebug>
#include <iostream>
#include <string>

using namespace std;

CircularImage::CircularImage(char initial, QWidget *parent)
    : QWidget{parent}
{
    this->initial = initial;
    this->setFixedWidth(CircularImage::PP_RADIUS_MID * 2);
    this->setFixedHeight(CircularImage::PP_RADIUS_MID * 2);
}
void CircularImage::paintEvent(QPaintEvent* event){
    QPainter painter(this);

    painter.setBrush(QBrush(MainScreen::USER_PP_CHAR_COLOR));
    painter.setPen(QPen(MainScreen::USER_PP_CHAR_COLOR));

    QString str = QString::fromStdString(string({this->initial}));
    painter.drawText(0, 0, CircularImage::CHAR_LENGTH, CircularImage::CHAR_LENGTH, 0, str );

    //qDebug() << "Debug: drawn QString in paintEvent of circular image is " << str;

    painter.drawArc(0, 0, CircularImage::PP_RADIUS_MID, CircularImage::PP_RADIUS_MID, 0, 360);
    painter.end();
}
