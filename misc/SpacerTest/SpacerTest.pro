TARGET = SpacerTest
TEMPLATE = app

QT += widgets svg charts
CONFIG += c++20

include(base.pri)

HEADERS += \
    ImpulseChart.h \
    SignalChart.h \
    SpacerTest.h \

SOURCES += \
    ImpulseChart.cpp \
    SignalChart.cpp \
    SpacerTest.cpp \

FORMS += \
    SpacerTest.ui



