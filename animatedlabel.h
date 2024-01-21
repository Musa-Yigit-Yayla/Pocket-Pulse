/*
* Pane regarding the animated label positioned on the top mid of the mainscreen
*/

#ifndef ANIMATEDLABEL_H
#define ANIMATEDLABEL_H

#include <QObject>
#include <QWidget>
#include <QString>
#include <QColor>
#include <QPaintEvent>
#include <QPainter>
#include <QRect>
#include <vector>
#include <QPainterPath>
#include <QPen>
#include <QTransform>

using namespace std;
class AnimatedLabel : public QWidget{
    Q_OBJECT
private:
    vector<int> pathPoints = {0, 35, 50, 35, 56, 10, 58, 60, 64, 35, 120, 35}; //coordinates of the path, in the form of {x0, y0, x1, y1, ..., xn - 1, yn - 1}
public:
    explicit AnimatedLabel(QWidget *parent = nullptr);
    void play();
    void stop();
    void paintEvent(QPaintEvent*) override;

    static const int HEIGHT = 80;
    static const int WIDTH = 120;
    static const int P_WIDTH = 40;
    static const int P_HEIGHT = HEIGHT;
    static const int P_LEFT_X = 52;
    static const int P_POINT_SIZE = 50;
    static const int POLYLINE_OFFSET_X = 15;
    static const int POLYLINE_OFFSET_Y = 24;
    static const int PULSE_WIDTH_SIZE = 4;
    static const QColor BACKGROUND_BLUE;
    static const QColor PATH_WHITE;
signals:

    class Pulse: public QWidget{

    };
};

#endif // ANIMATEDLABEL_H
