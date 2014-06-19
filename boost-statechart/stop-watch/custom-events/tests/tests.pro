TEMPLATE = app
TARGET = run-tests

CONFIG += testcase
QT += testlib

INCLUDEPATH += ../components

# Input
HEADERS += ../components/stop-watch.h
SOURCES += test_stop-watch.cpp

CONFIG += c++11
QMAKE_CXXFLAGS += -Werror
