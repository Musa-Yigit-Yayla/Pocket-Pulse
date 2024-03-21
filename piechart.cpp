#include "piechart.h"
using namespace std;

PieChart::PieChart(QWidget* parent): QWidget{parent}
{

}
PieChart::PieChart(vector<double>& contentValues, vector<string>& contentHeaders, QWidget* parent): QWidget{parent}{
    this->setContents(contentValues, contentHeaders);
}
PieChart::PieChart(vector<double>& contentValues, vector<string>& contentHeaders, int width, int height, QWidget* parent):
    PieChart{contentValues, contentHeaders, parent}{
    this->setFixedSize(width, height);
}

vector<double> PieChart::getContentValues() const{
    return this->contentValues; //return by copy
}
vector<string> PieChart::getContentHeaders() const{
    return this->contentHeaders; //return by copy
}
void PieChart::setContents(vector<double>& contentValues, vector<string>& contentHeaders){
    this->contentValues = contentValues;
    this->contentHeaders = contentHeaders;

    //refresh the contents with the current displayValueEnabled value
    this->refresh(this->displayValuesEnabled);
}
int PieChart::getWidth() const{
    return this->width();
}
int PieChart::getHeight() const{
    return this->height();
}
//minimum 100 for both width and height
void PieChart::setSize(int width, int height){
    if(width < 100){
        width = 100;
    }
    if(height < 100){
        height = 100;
    }
    this->setFixedSize(width, height);
}
//refreshes the piechart by first setting displayValues option
void PieChart::refresh(bool displayValues){
    this->displayValuesEnabled = displayValues;
    this->update();
}
void PieChart::paintEvent(QPaintEvent* event){
    QPainter painter(this);

    const int RECT_LENGTH = 10;
    const int RADIUS = this->width() / 2 - RECT_LENGTH; // circle radius

    //sum the total values
    double totalSum = 0;
    double angleSum = 0;

    for(double d: this->contentValues){
        totalSum += ((double)((int)(d * 100))) / 100;
    }


    int currX = RADIUS;
    int currY = 0;


    if(this->contentValues.size() == this->contentHeaders.size()){ //just to enable the program to remain fault tolerant
        vector<QColor> usedColors; //colors which have been used so far
        //start from the top point of the circle tangent to the x axis and draw slices clockwise
        for(int i = 0; i < this->contentValues.size(); i++){
            double currValue = this->contentValues.at(i);
            double currAngle = (-360 * currValue) / totalSum; //we have the curr angle in negative degrees

            //generate a random color which has not been used before
            QColor newColor;
            do{
                newColor = QColor(rand() % 200 + 55, rand() % 200 + 55 , rand() % 200 + 55);
                qDebug() << "Debug: do while loop body executed and newColor is " << newColor;
            }while(count(usedColors.begin(), usedColors.end(), newColor) != 0);

            painter.setPen(newColor);
            usedColors.push_back(newColor);


            //recall that zero degree is at 3 o'clock position and degrees go with 1/16 precision and positive degrees mean counter clockwise
            painter.drawArc(currX, currY, RADIUS, RADIUS, angleSum, currAngle);
            angleSum += currAngle; //angleSum also contains negative values now
        }
    }

}
