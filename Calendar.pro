#-------------------------------------------------
#
# Project created by QtCreator 2016-08-25T19:13:53
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Calendar
TEMPLATE = app

TRANSLATIONS += chinese.ts\
                qt_zh_CN.ts

SOURCES += main.cpp\
        mainwindow.cpp \
    mycalendar.cpp \
    schedulemanager.cpp \
    scheduledialog.cpp \
    schedule.cpp \
    listwidget.cpp \
    filesdialog.cpp

HEADERS  += mainwindow.h \
    mycalendar.h \
    utility.h \
    schedulemanager.h \
    scheduledialog.h \
    schedule.h \
    listwidget.h \
    filesdialog.h

FORMS    += mainwindow.ui \
    scheduledialog.ui \
    filesdialog.ui

DISTFILES +=

RESOURCES += \
    resource.qrc
