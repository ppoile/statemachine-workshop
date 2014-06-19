TEMPLATE = app
TARGET = run-tests

CONFIG += console testcase
QT += testlib

INCLUDEPATH += . ../components

SOURCES += \
    test_stop-watch.cpp \

HEADERS += \
    ../components/stop-watch.h

CONFIG += c++11
