QT       += core gui \
            sql \
            network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/'../../../../../../Program Files/MySQL/MySQL Connector C 6.1/lib/' -llibmysql
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/'../../../../../../Program Files/MySQL/MySQL Connector C 6.1/lib/' -llibmysqld
#else:unix: LIBS += -L$$PWD/'../../../../../../Program Files/MySQL/MySQL Connector C 6.1/lib/' -llibmysql
#INCLUDEPATH += $$PWD/'../../../../../../Program Files/MySQL/MySQL Connector C 6.1/include/mysql'
#DEPENDPATH += $$PWD/'../../../../../../Program Files/MySQL/MySQL Connector C 6.1/include/mysql'


SOURCES += \
    MainScreen.cpp \
    abstractpane.cpp \
    animatedlabel.cpp \
    bankingcontroller.cpp \
    bankpane.cpp \
    circularimage.cpp \
    contactspane.cpp \
    debtpane.cpp \
    expensepane.cpp \
    fingoalpane.cpp \
    incomepane.cpp \
    loginscreen.cpp \
    main.cpp \
    maincontroller.cpp \
    networkcontroller.cpp \
    progressrectangle.cpp \
    registerscreen.cpp \
    savecodepane.cpp \
    user.cpp

HEADERS += \
    abstractpane.h \
    animatedlabel.h \
    bankingcontroller.h \
    bankpane.h \
    circularimage.h \
    contactspane.h \
    debtpane.h \
    expensepane.h \
    fingoalpane.h \
    incomepane.h \
    loginscreen.h \
    maincontroller.h \
    mainscreen.h \
    networkcontroller.h \
    progressrectangle.h \
    registerscreen.h \
    savecodepane.h \
    user.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


