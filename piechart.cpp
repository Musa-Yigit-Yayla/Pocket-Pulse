#include "piechart.h"
#include "rectwidget.h"

using namespace std;

PieChart::PieChart(QWidget* parent): QWidget{parent}
{

    this->setSize(800, 500);
    //initialize layout management
    QWidget* intermediateWrapper = new QWidget(this);
    this->hboxHeaders->setSpacing(20);
    this->hboxHeaders->setSizeConstraint(QLayout::SetFixedSize);
    intermediateWrapper->setLayout(this->hboxHeaders);
    this->headerSA->setWidget(intermediateWrapper);
    this->headerSA->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    this->headerSA->setFixedWidth(300);
    this->headerSA->setFixedHeight(150);
    QWidget* intermediateWrapper2 = new QWidget(this);
    intermediateWrapper2->setLayout(this->vboxValues);
    this->vboxValues->setSpacing(20);
    this->vboxValues->setSizeConstraint(QLayout::SetFixedSize);
    this->valuesSA->setWidget(intermediateWrapper2);
    this->valuesSA->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    this->valuesSA->setFixedSize(200, 400);
    wrapper->addWidget(this);
    wrapper->addWidget(this->headerSA);
    wrapper->addWidget(this->valuesSA);
}
PieChart::PieChart(vector<double>& contentValues, vector<string>& contentHeaders, QWidget* parent): PieChart{parent}{
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
QVBoxLayout* PieChart::getContainerLayout(){
    return this->wrapper;
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
    this->redrawHeadersEnabled = true;
    this->update();
}
void PieChart::paintEvent(QPaintEvent* event){
    QPainter painter(this);
    qDebug() << "Debug: PieChart::refresh has been invoked with event type " << event->type();


    const int RECT_LENGTH = 20;
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
        //clear out the current contents of the rect hbox and valuesvbox if the event is not a scroll event
        if(this->redrawHeadersEnabled){
            while(this->hboxHeaders->count() > 0){ //&& !(event->type() == QEvent::Scroll || event->type() == QEvent::MouseButtonPress)){
                QLayout* layout = this->hboxHeaders->takeAt(0)->layout();

                //delete the contents of the layout (we have 2 children normally) and all children must be of QWidget subclass
                while(layout->count() > 0){
                    QWidget* currChild = layout->takeAt(0)->widget();
                    delete currChild;
                }
                qDebug() << "Debug: PieChart::paintEvent hboxHeaders child layout to be deleted has pointer value " << layout;
                delete layout;
            }

            //also clear out the vboxValues' contents
            while(this->vboxValues->count() > 0){
                QWidget* widget = this->vboxValues->takeAt(0)->widget(); //widget child is expected
                delete widget;
            }
        }

        double sumValue = 0;

        vector<QColor> usedColors; //colors which have been used so far
        //start from the top point of the circle tangent to the x axis and draw slices clockwise
        for(int i = 0; i < this->contentValues.size(); i++){
            double currValue = this->contentValues.at(i);
            if(currValue == 0){
                continue;
            }
            double currAngle = (-360 * currValue * 16) / totalSum; //we have the curr angle in negative degrees

            //generate a random color which has not been used before
            QColor newColor;
            do{
                newColor = QColor(rand() % 200 + 30, rand() % 200 + 30 , rand() % 200 + 30);
            }while(count(usedColors.begin(), usedColors.end(), newColor) != 0);

            if(this->displayValuesEnabled && this->redrawHeadersEnabled){
                //add a small rectangle along with a QLabel into our hbox which is wrapped in the scroll area
                RectWidget* rect = new RectWidget(RECT_LENGTH, RECT_LENGTH, newColor, this);
                QVBoxLayout* rectWrapper = new QVBoxLayout(this);
                rectWrapper->addWidget(rect);
                QLabel* headerLabel = new QLabel(QString::fromStdString(this->contentHeaders.at(i)), this);
                rectWrapper->addWidget(headerLabel);

                //add the vbox into the hbox
                this->hboxHeaders->addLayout(rectWrapper);

                //now update the valuesSA content (vboxValues)
                QLabel* valueLabel = new QLabel(QString::fromStdString(this->contentHeaders.at(i) + ": " + to_string(currValue) + "$"), this);
                this->vboxValues->addWidget(valueLabel);
                sumValue += currValue;
            }

            painter.setPen(newColor);
            painter.setBrush(newColor);
            usedColors.push_back(newColor);

            //recall that zero degree is at 3 o'clock position and degrees go with 1/16 precision and positive degrees mean counter clockwise
            painter.drawPie(currX, currY, RADIUS, RADIUS, angleSum, currAngle);
            angleSum += currAngle; //angleSum also contains negative values now
        }
        //add the total sum value to the vboxValues if conditions are satisfied
        if(this->displayValuesEnabled && this->redrawHeadersEnabled){
            QLabel* totalLabel = new QLabel(QString::fromStdString("Total: " + to_string(sumValue) + "$"), this);
            this->vboxValues->addWidget(totalLabel);
        }
    }
    //set the redrawHeadersEnabled to its default state
    this->redrawHeadersEnabled = false;

}
