#include <QRect>
#include <QWidget>
#include <cstddef>
#include <QVBoxLayout>
#include <string>
#include <QString>
#include <QPainter>

#ifndef PROGRESSRECTANGLE_H
#define PROGRESSRECTANGLE_H

class ProgressRectangle: public QWidget{
private:
    int height;
    int width;
    double fillRatio;

public:
    ProgressRectangle(const int width, const int height, double fillRatio, QWidget* parent = nullptr);
    void redraw(double fillRatio);
    static QColor getFillRatioRGB(double fillRatio);
    void paintEvent(QPaintEvent* event) override;
};

#endif // PROGRESSRECTANGLE_H
