QT        += core gui widgets

TARGET    = example
TEMPLATE  = app

CONFIG    += c++11

RESOURCES += resource.qrc

HEADERS   += ../src/burgermenu.h

SOURCES   += main.cpp\
             ../src/burgermenu.cpp
