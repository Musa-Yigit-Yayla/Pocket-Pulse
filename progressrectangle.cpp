#include "progressrectangle.h"

using namespace std;
//Fill ratio lies between [0, 1]
ProgressRectangle::ProgressRectangle(const int width, const int height, const double fillRatio, QWidget* parent): QWidget{parent}{
    this->width = width;
    this->height = height;
    this->fillRatio = fillRatio;

}
void ProgressRectangle::redraw(const double fillRatio){
    this->fillRatio = this->fillRatio;
    if(this->upperRect != NULL && this->lowerRect != NULL){
        delete this->upperRect;
        delete this->lowerRect;
    }
    double lowerHeight = this->height * fillRatio;
    double upperHeight = this->height - lowerHeight;

    this->upperRect = new QRect();
    this->upperRect->setWidth(this->width);
    this->upperRect->setHeight(upperHeight);

    this->lowerRect->setWidth(this->width);
    this->lowerRect->setHeight(lowerHeight);
    //set the fill color of the lowerRect wrt fillratio
    int* color = getFillRatioRGB(this->fillRatio);
    string r = to_string(color[0]), b = to_string(color[1]), g = to_string(color[2]);
    //this->lowerRect->setStyleSheet(QString::fromStdString("rgb(" + r + ", " + b + ", " + g + ")"));
}
/**
 * @brief ProgressRectangle::getFillRatioRGB
 * Computes an rgb value wrt given fillRatio (0 returns full green, 1 returns full red)
 *
 * @param fillRatio [0, 1]
 * @return Returns a dynamically allocated int array of length 3, containing values for R, G, B respectively
 */
int* ProgressRectangle::getFillRatioRGB(const double fillRatio){
    //set blue value to 0
    int* result = new int[3];
    int r = 255 * fillRatio;
    int g = 255 - r;
    int b = 20;

    result[0] = r;
    result[1] = b;
    result[2] = g;
    return result;
}
