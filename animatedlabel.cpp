#include "animatedlabel.h"
#include <string>

using namespace std;
AnimatedLabel::AnimatedLabel(QWidget *parent)
    : QWidget{parent}
{
    this->pulse = new Pulse(5000); //ensure you pass the parent as the outer class
}
void AnimatedLabel::play(){

}
void AnimatedLabel::stop(){

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

    QPainterPath path;
    QPen initialPen = painter.pen();
    QPen pen;
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
    painter.setPen(initialPen);

    QFont font = painter.font();
    font.setPointSize(AnimatedLabel::P_POINT_SIZE);
    font.setItalic(true);
    painter.setFont(font);
    painter.drawText(P_LEFT_X, 0, AnimatedLabel::P_WIDTH, AnimatedLabel::P_HEIGHT, 0, str);

    //paint the pulse if it's active
    if(this->pulse != NULL && this->pulse->isActive()){
        //painter.setBrush(AnimatedLabel::Pulse::PULSE_ORANGE);
        //painter.setPen(AnimatedLabel::Pulse::PULSE_ORANGE);

        painter.fillRect(this->pulse->getRect(), AnimatedLabel::Pulse::PULSE_ORANGE);
        //alter the x and y coordinates of the pulse
        vector<int> coordinates = this->pulse->getCoordinates();
        int currX = coordinates.at(0);
        int currY = coordinates.at(1);

        int incX = 0, incY = 0; //incremental rates
        int duration = this->pulse->getDurationMillis();
        int stateDuration;
        //state time distribution is as follows: {s0 = 0.3, s1 = 0.1, s2 = 0.2, s3 = 0.1, s4 = 0.3}
        switch(this->pulse->getState()){
        case 0:
            stateDuration = duration * 0.3;
            incX = (pathPoints.at(2) - pathPoints.at(0)) / stateDuration;
            incY = 0; break;
        case 1:
            stateDuration = duration * 0.1;
            incX = (pathPoints.at(4) - pathPoints.at(2)) / stateDuration;
            incY = (pathPoints.at(5) - pathPoints.at(3)) / stateDuration; break;
        case 2:
            stateDuration = duration *  0.2;
            incX =(pathPoints.at(6) - pathPoints.at(4));
            incY = (pathPoints.at(7) - pathPoints.at(5)); break;
        }
        this->pulse->setCoordinates(currX + incX, currY + incY);
    }
    painter.end();
}
void AnimatedLabel::repaintSlot() {
    this->update();
}
AnimatedLabel::Pulse::Pulse(int durationMillis, QWidget* parent): QObject{parent}{
    this->durationMillis = durationMillis;

    //bind the timer with paintEvent of the outer class
    AnimatedLabel* al = reinterpret_cast<AnimatedLabel*>(this->parent());
    QObject::connect(&(this->timer), &QTimer::timeout, al, &AnimatedLabel::repaintSlot);
}
bool AnimatedLabel::Pulse::isActive() const{
    return this->active;
}
void AnimatedLabel::Pulse::play(){
    this->active = true;
}
void AnimatedLabel::Pulse::stop(){
    this->active = false;
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
    this->currX = newX;
    this->currY = newY;

    //Don't forget to update the state when applicable
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
const QColor AnimatedLabel::BACKGROUND_BLUE = QColor(6, 59, 135);
const QColor AnimatedLabel::PATH_WHITE = QColor(242, 240, 230);
const QColor AnimatedLabel::Pulse::PULSE_ORANGE = QColor(251, 185, 9);
