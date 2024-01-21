#include "circularimage.h"
#include "mainscreen.h"
#include <qDebug>
#include <iostream>
#include <string>
#include <cmath>

using namespace std;

CircularImage::CircularImage(char initial, QWidget *parent)
    : QWidget{parent}
{
    this->initial = initial;
    this->setFixedWidth(CircularImage::FONT_SIZE_MID * 2);
    this->setFixedHeight(CircularImage::FONT_SIZE_MID * 2);
}
void CircularImage::paintEvent(QPaintEvent* event){
    QPainter painter(this);

    painter.setBrush(QBrush(MainScreen::USER_PP_CHAR_COLOR));
    painter.setPen(QPen(MainScreen::USER_PP_CHAR_COLOR));

    QString str = QString::fromStdString(string({this->initial}));
    QFont font = painter.font();
    qDebug() << "Debug: default point size of the font is " << font.pointSize();
    font.setPointSize(FONT_SIZE_MID);
    painter.setFont(font);

    painter.drawText(0, 0, CircularImage::FONT_SIZE_MID * 2, CircularImage::FONT_SIZE_MID * 2, 0, str );

    qDebug() << "Debug: drawn QString in paintEvent of circular image is " << str;

    painter.drawArc(0, 0, CircularImage::FONT_SIZE_MID * 3, CircularImage::FONT_SIZE_MID * 3, 0, M_PI * 2);
    painter.end();
}
