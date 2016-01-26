#-------------------------------------------------
#
# Project created by QtCreator 2015-12-19T16:54:48
#
#-------------------------------------------------

QT += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = QuadUI
TEMPLATE = app

win32:contains(QMAKE_HOST.arch, x86_64) {
  LIBS += -L$$PWD/Libs/64 -lxinput -lOpenGL32
} else:win32:contains(QMAKE_HOST.arch, x86) {
  LIBS += -L$$PWD/Libs/32 -lxinput -lOpenGL32
} else:unix {
  message(Linux is not supported)
}

SOURCES += main.cpp\
        mainwindow.cpp \
        joystick.cpp \
        qcustomplot.cpp \
        wifi.cpp \
        visualiser.cpp

HEADERS  += mainwindow.h \
        joystick.h \
        qcustomplot.h \
        wifi.h \
        visualiser.h

FORMS    += mainwindow.ui
