/**
 * Class for representing the initial of the user drawn on a circle
*/

#ifndef CIRCULARIMAGE_H
#define CIRCULARIMAGE_H

#include <QWidget>
#include <QPaintEvent>
#include <QPainter>
#include <QFont>
#include <string>

using namespace std;
class CircularImage : public QWidget{
    Q_OBJECT
private:
    string imgPath;
    char initial;
public:
    CircularImage(char initial, QWidget *parent = nullptr);
    void paintEvent(QPaintEvent* event) override;

    static const int PP_RADIUS_MID = 12;
    static const int CHAR_LENGTH = 16;
    static const int FONT_SIZE_MID = 25;
signals:

};

#endif // CIRCULARIMAGE_H
