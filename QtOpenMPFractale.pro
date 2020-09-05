
QT       += core gui

QMAKE_CXXFLAGS += -std=c++17


# CONFIG += debug
CONFIG += release

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ./bin/exec
TEMPLATE = app

OBJECTS_DIR = ./obj
MOC_DIR = ./obj

###

SOURCES +=              \
    src/main.cpp        \
    src/mainwindow.cpp  \
    src/Fractale.cpp

###

HEADERS += \
    src/mainwindow.hpp  \
    src/Fractale.hpp

###

FORMS   +=
LIBS    +=

INCLUDEPATH += $$PWD/src
DEPENDPATH += $$PWD/src




QMAKE_CXXFLAGS += -fopenmp
LIBS += -fopenmp
