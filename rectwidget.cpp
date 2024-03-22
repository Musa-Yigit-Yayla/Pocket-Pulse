#include "rectwidget.h"

using namespace std;

RectWidget::RectWidget(int w, int h, QColor& color, QWidget *parent): QWidget{parent}{
    this->refresh(w, h, color);
}

void RectWidget::refresh(int w, int h, QColor& color){
    this->color = color;
    this->setFixedSize(w + 10, h + 10);
}
void RectWidget::paintEvent(QPaintEvent* event){
    QPainter painter(this);
    int width = this->width();
    int height = this->height();

    painter.fillRect(0, 0, width, height, this->color);
}
