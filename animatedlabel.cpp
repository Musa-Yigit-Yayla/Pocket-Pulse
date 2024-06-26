#include "animatedlabel.h"
#include <string>

using namespace std;
AnimatedLabel::AnimatedLabel(QWidget *parent)
    : QWidget{parent}
{
    this->pulse = new Pulse(80, this); //ensure you pass the parent as the outer class
    this->play();
}
void inline AnimatedLabel::play(){
    this->pulse->play();
}
void inline AnimatedLabel::stop(){
    this->pulse->stop();
}
void AnimatedLabel::paintEvent(QPaintEvent* event){
    QPainter painter(this);
    //fill the whole rectangle as blue
    painter.setBrush(QBrush(BACKGROUND_BLUE));
    painter.setPen(QPen(BACKGROUND_BLUE));
    painter.fillRect(QRect(0, 0, AnimatedLabel::WIDTH, AnimatedLabel::HEIGHT), painter.brush());
    //draw the full path and the char P
    painter.setBrush(QBrush(BACKGROUND_BLUE));
    painter.setPen(QPen(PATH_WHITE));

    QString str = "P";
    QPen pen;
    pen.setBrush(QBrush(LOGO_ORANGE));
    painter.setPen(pen);
    QFont font = painter.font();
    font.setPointSize(AnimatedLabel::P_POINT_SIZE);
    font.setItalic(true);
    painter.setFont(font);

    painter.drawText(P_LEFT_X, 0, AnimatedLabel::P_WIDTH, AnimatedLabel::P_HEIGHT, 0, str);
    QPainterPath path;
    QPen initialPen = painter.pen();

    pen.setWidth(AnimatedLabel::PULSE_WIDTH_SIZE);
    pen.setBrush(QBrush(PATH_WHITE));
    painter.setPen(pen);
    path.moveTo(this->pathPoints.at(0), this->pathPoints.at(1) + POLYLINE_OFFSET_Y);

    for(int i = 2; i < this->pathPoints.size() - 1; i += 2){
        int x0 = pathPoints.at(i), y0 = pathPoints.at(i + 1);
        path.lineTo(x0 + POLYLINE_OFFSET_X, y0 + POLYLINE_OFFSET_Y);
    }
    painter.drawPath(path);

    //reset the QPen to its initial state

    //pen.setBrush(QBrush(PATH_WHITE));

    //paint the pulse if it's active
    if(this->pulse != NULL && this->pulse->isActive()){
        int Y_OFFSET = 22;

        //alter the x and y coordinates of the pulse
        vector<int> coordinates = this->pulse->getCoordinates();
        int currX = coordinates.at(0);
        int currY = coordinates.at(1);

        QColor fillColor = AnimatedLabel::Pulse::PULSE_PURPLE;

        painter.fillRect(QRect(currX, currY + Y_OFFSET, this->pulse->getWidth(), this->pulse->getHeight()), fillColor);

        int incX = 0, incY = 0; //incremental rates
        //state time distribution is as follows: {s0 = 0.3, s1 = 0.1, s2 = 0.2, s3 = 0.1, s4 = 0.3}
        switch(this->pulse->getState()){
        case 0:
            incX = 6;
            incY = 0; break;
        case 1:
            incX = 1;
            incY = -3;break;
        case 2:
            incX = 1;
            incY = 8;break;
        case 3:
            incX = 1;
            incY = -3; break;
        case 4:
            incX = 12;
            currY = this->pathPoints.at(pathPoints.size() - 1);
            incY = 0; break;
        }
        this->pulse->setCoordinates(currX + incX, currY + incY);
    }
    painter.end();
}
void AnimatedLabel::repaintSlot() {
    //qDebug() << "Debug: repaintSlot of animated label has been invoked";
    this->update();
}
AnimatedLabel::Pulse::Pulse(int durationMillis, QWidget* parent): QObject{parent}{
    this->durationMillis = durationMillis;
    this->timer = new QTimer(this);
    this->timer->setInterval(this->durationMillis);

    //bind the timer with paintEvent of the outer class
    AnimatedLabel* al = reinterpret_cast<AnimatedLabel*>(this->parent());
    QObject::connect(this->timer, &QTimer::timeout, al, &AnimatedLabel::repaintSlot);
}
AnimatedLabel::Pulse::~Pulse(){
    delete this->timer;
}
bool AnimatedLabel::Pulse::isActive() const{
    return this->active;
}
void AnimatedLabel::Pulse::play(){
    this->active = true;
    this->timer->start();
}
void AnimatedLabel::Pulse::stop(){
    this->active = false;
    this->timer->stop();
}
inline int AnimatedLabel::Pulse::getWidth() const{
    return this->width;
}
inline int AnimatedLabel::Pulse::getHeight() const{
    return this->height;
}
void AnimatedLabel::Pulse::setDurationMillis(int durationMillis){
    if(durationMillis > 0){
        this->durationMillis = durationMillis;
    }
}
int AnimatedLabel::Pulse::getDurationMillis() const{
    return this->durationMillis;
}
vector<int> AnimatedLabel::Pulse::getCoordinates() const{
    vector<int> vec = {this->currX, this->currY};
    return vec;
}
void AnimatedLabel::Pulse::setCoordinates(int newX, int newY){
    if(newX < 0){
        newX = 0;
    }
    if(newY < 10){
        newY = 10;
    }
    else if(newY > 54){
        newY = 54;
    }
    this->currX = newX;
    this->currY = newY;

    //update the state when applicable

    if(this->currX < statePoints.at(0)){
        //state 0
        this->state = 0;
    }
    else if(this->currX < statePoints.at(1)){
        this->state = 1;
    }
    else if(this->currX < statePoints.at(2)){
        this->state = 2;
    }
    else if(this->currX < statePoints.at(3)){
        this->state = 3;
    }
    else if(this->currX < statePoints.at(4)){
        this->state = 4;
    }
    else{
        //restart the animation
        this->currX = pathPoints.at(0);
        this->currY = pathPoints.at(1);
        this->state = 0;
    }
    //qDebug() << "The new coordinates are {" << this->currX << ", " << this->currY << " and the new state is " << this->state;
}
QRect AnimatedLabel::Pulse::getRect() const{
    return QRect(this->currX, this->currY, this->width, this->height);
}
void AnimatedLabel::Pulse::setState(int state){
    this->state = state;
}
int AnimatedLabel::Pulse::getState() const{
    return this->state;
}
const vector<int> AnimatedLabel::pathPoints = {0, 35, 50, 35, 56, 10, 67, 60, 72, 35, 120, 35};
const vector<int> AnimatedLabel::Pulse::statePoints = {55, 70, 80, 86, 130};
const QColor AnimatedLabel::BACKGROUND_BLUE = QColor(6, 59, 135);
const QColor AnimatedLabel::PATH_WHITE = QColor(242, 240, 230);
const QColor AnimatedLabel::LOGO_ORANGE = QColor(253, 106, 2);
const QColor AnimatedLabel::Pulse::PULSE_ORANGE = QColor(251, 185, 9);
const QColor AnimatedLabel::Pulse::PULSE_PURPLE = QColor(214, 32, 134);


