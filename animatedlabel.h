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
#include <QTimer>
#include <cstddef>
#include <random>

using namespace std;
class AnimatedLabel : public QWidget{
    Q_OBJECT
private:
    static const vector<int> pathPoints; //coordinates of the path, in the form of {x0, y0, x1, y1, ..., xn - 1, yn - 1}

    static const int HEIGHT = 80;
    static const int WIDTH = 120;
    static const int P_WIDTH = 40;
    static const int P_HEIGHT = HEIGHT;
    static const int P_LEFT_X = 52;
    static const int P_POINT_SIZE = 50;
    static const int POLYLINE_OFFSET_X = 15;
    static const int POLYLINE_OFFSET_Y = 24;
    static const int PULSE_WIDTH_SIZE = 4;
public:
    explicit AnimatedLabel(QWidget *parent = nullptr);
    void inline play();
    void inline stop();
    void paintEvent(QPaintEvent*) override;

    static const QColor BACKGROUND_BLUE;
    static const QColor PATH_WHITE;
    static const QColor LOGO_ORANGE;
    static const int ANIMATION_DURATION_SEC = 5;

    class Pulse: public QObject{
        //use the scope resolution operator twice to implement the methods of the Pulse class
    private:
        int width = 5;
        int height = 5;
        int durationMillis;
        bool active = false;
        int currX = pathPoints.at(0);
        int currY = pathPoints.at(1);
        int state = 0; //5 states {0, 1, 2, 3, 4} which determine where we are positioned along the pulse like path
        QTimer* timer = nullptr;
        static const vector<int> statePoints;

    public:
        Pulse(int durationMillis, QWidget* parent = nullptr);
        ~Pulse();
        bool isActive() const;
        void play();
        void stop();
        inline int getWidth() const;
        inline int getHeight() const;
        void setDurationMillis(int durationMillis);
        int getDurationMillis() const;
        vector<int> getCoordinates() const;
        void setCoordinates(int newX, int newY);
        QRect getRect() const;
        void setState(int state);
        int getState() const;

        static const QColor PULSE_ORANGE;
        static const QColor PULSE_PURPLE;

    };
private:
    Pulse* pulse = nullptr;;
signals:

public slots:
    void repaintSlot();

};

#endif // ANIMATEDLABEL_H
