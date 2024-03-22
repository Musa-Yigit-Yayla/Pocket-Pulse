#ifndef RECTWIDGET_H
#define RECTWIDGET_H

#include <QWidget>
#include <QPaintEvent>
#include <QPainter>
#include <QColor>

class RectWidget : public QWidget{
    Q_OBJECT
private:
    QColor color;
public:
    RectWidget(int w, int h, QColor& color, QWidget *parent = nullptr);

    void refresh(int w, int h, QColor& color);
    void paintEvent(QPaintEvent* event) override;
signals:

};

#endif // RECTWIDGET_H
