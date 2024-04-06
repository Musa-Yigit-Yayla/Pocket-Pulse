#include "abstractpane.h"
#include "user.h"

using namespace std;

#ifndef HOMEPANE_H
#define HOMEPANE_H

class HomePane: AbstractPane{
public:
    HomePane(User* user, QWidget* parent = nullptr);
};

#endif // HOMEPANE_H
