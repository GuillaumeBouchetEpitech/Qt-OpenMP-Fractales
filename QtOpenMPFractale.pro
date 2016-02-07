
QT       += core gui

QMAKE_CXXFLAGS += -std=c++11


CONFIG += debug


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = exec
TEMPLATE = app

OBJECTS_DIR = ./obj
MOC_DIR = ./obj

###

SOURCES +=          	\
    src/main.cpp    	\
    src/mainwindow.cpp	\
    src/Fractale.cpp	\
    src/helpers/HelperOpenMP.cpp

###

HEADERS  += \
    src/mainwindow.hpp	\
    src/Fractale.hpp	\
    src/helpers/HelperOpenMP.hpp

###

FORMS   +=
LIBS    +=

INCLUDEPATH += $$PWD/src
DEPENDPATH += $$PWD/src




QMAKE_CXXFLAGS += -fopenmp
LIBS += -fopenmp
