#include "abstractpane.h"

#ifndef REPORTPANE_H
#define REPORTPANE_H


class ReportPane: public AbstractPane{
public:
    ReportPane(User* user, QWidget* parent = nullptr);
};

#endif // REPORTPANE_H
