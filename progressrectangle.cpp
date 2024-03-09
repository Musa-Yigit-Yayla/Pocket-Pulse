#include "progressrectangle.h"

using namespace std;
//Fill ratio lies between [0, 1]
ProgressRectangle::ProgressRectangle(const int width, const int height, double fillRatio, QWidget* parent): QWidget{parent}{
    qDebug() << "Debug: ProgressRectangle constructor has been invoked with fillRatio " << fillRatio;
    this->width = width;
    this->height = height;

    this->fillRatio = fillRatio;

    this->setFixedSize(this->width, this->height);
    this->redraw(this->fillRatio);
}
void ProgressRectangle::redraw(double fillRatio){
    if(fillRatio < 0){
        fillRatio = 0.0;
    }
    else if(fillRatio > 1){
        fillRatio = 1.0;
    }
    this->fillRatio = fillRatio;
    this->update();
}
/**
 * @brief ProgressRectangle::getFillRatioRGB
 * Computes an rgb value wrt given fillRatio (0 returns full green, 1 returns full red)
 *
 * @param fillRatio [0, 1]
 * @return Returns a QColor
 */
QColor ProgressRectangle::getFillRatioRGB(double fillRatio){
    if(fillRatio < 0){
        fillRatio = 0.0;
    }
    else if(fillRatio > 1){
        fillRatio = 1.0;
    }

    //set blue value to 0
    int r = 255 * fillRatio;
    int g = 255 - r;
    int b = 20;

    QColor color(r, g, b);
    return color;
}
void ProgressRectangle::paintEvent(QPaintEvent* event){
    QPainter painter(this);
    double lowerHeight = this->height * fillRatio;
    double upperHeight = this->height - lowerHeight;

    //set the fill color of the lowerRect wrt fillratio
    QColor fillColor = getFillRatioRGB(this->fillRatio);
    QBrush emptyColor = painter.brush();
    painter.setBrush(QBrush(fillColor));


    //draw the lower rect
    painter.drawRect(QRect(0, upperHeight, this->width, lowerHeight));

    //draw the upper rect
    painter.setBrush(emptyColor); //restore brush
    painter.drawRect(QRect(0, 0, this->width, upperHeight));
}
