TARGET = SpacerTest
TEMPLATE = app

QT += widgets svg charts
CONFIG += c++20

INCLUDEPATH += ../_common

HEADERS += \
    ../_common/MathGeneral.h \
    ../_common/MathGeneral.hpp \
    ../_common/MathVector3.h \
    ../_common/MathVector3.hpp \
    ../_common/Tools.h \
    ../_common/Tools.hpp

HEADERS += \
    SpacerTest.h \
    SpatialFunction.h \
    SpatialRingBuffer.h 

SOURCES += \
    SpacerTest.cpp \
    SpatialFunction.cpp \
    SpatialRingBuffer.cpp

FORMS += \
    SpacerTest.ui



