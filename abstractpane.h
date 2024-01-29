#ifndef ABSTRACTPANE_H
#define ABSTRACTPANE_H

#include <QWidget>
#include <cstddef>

class AbstractPane: public QWidget
{
public:
    AbstractPane(QWidget* parent = nullptr);
    virtual ~AbstractPane(); //virtual destructor prevents the class from being instantiated

    static const int PANE_WIDTH = 1850;
    static const int PANE_HEIGHT = 980;
};

#endif // ABSTRACTPANE_H
