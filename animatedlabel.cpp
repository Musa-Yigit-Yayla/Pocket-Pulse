#include "animatedlabel.h"
#include <string>

using namespace std;
AnimatedLabel::AnimatedLabel(QWidget *parent)
    : QWidget{parent}
{
    this->pulse = new Pulse(5000, this); //ensure you pass the parent as the outer class
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
    }
    painter.end();
}
void AnimatedLabel::repaintSlot() {
    this->update();
}
AnimatedLabel::Pulse::Pulse(int durationMillis, QWidget* parent): QWidget{parent}{
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
void AnimatedLabel::Pulse::paintEvent(QPaintEvent* event){

}
vector<int> AnimatedLabel::Pulse::getCoordinates() const{
    vector<int> vec = {this->currX, this->currY};
    return vec;
}
void AnimatedLabel::Pulse::setCoordinates(int newX, int newY){
    this->currX = newX;
    this->currY = newY;
}
QRect AnimatedLabel::Pulse::getRect() const{
    return QRect(this->currX, this->currY, this->width, this->height);
}
const QColor AnimatedLabel::BACKGROUND_BLUE = QColor(6, 59, 135);
const QColor AnimatedLabel::PATH_WHITE = QColor(242, 240, 230);
const QColor AnimatedLabel::Pulse::PULSE_ORANGE = QColor(251, 185, 9);
