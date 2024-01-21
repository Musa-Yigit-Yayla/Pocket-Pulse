#include "animatedlabel.h"
#include <string>

using namespace std;
AnimatedLabel::AnimatedLabel(QWidget *parent)
    : QWidget{parent}
{

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

}

const QColor AnimatedLabel::BACKGROUND_BLUE = QColor(6, 59, 135);
const QColor AnimatedLabel::PATH_WHITE = QColor(242, 240, 230);
