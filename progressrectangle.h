#include <QRect>
#include <QWidget>
#include <cstddef>
#include <QVBoxLayout>
#include <string>
#include <QString>

#ifndef PROGRESSRECTANGLE_H
#define PROGRESSRECTANGLE_H


class ProgressRectangle: public QWidget{
private:
    int height;
    int width;
    double fillRatio;

    QVBoxLayout* vbox = new QVBoxLayout(this); //vbox to contain the rectangles
    QRect* upperRect = nullptr;
    QRect* lowerRect = nullptr;
public:
    ProgressRectangle(const int width, const int height, const double fillRatio, QWidget* parent = nullptr);
    void redraw(const double fillRatio);
    static int* getFillRatioRGB(const double fillRatio);
};

#endif // PROGRESSRECTANGLE_H
