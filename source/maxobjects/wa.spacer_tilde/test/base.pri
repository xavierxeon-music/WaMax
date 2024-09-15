INCLUDEPATH += \
    $$PWD/.. \
    $$PWD/../../_common

HEADERS += \
    $$PWD/../../_common/MathGeneral.h \
    $$PWD/../../_common/MathGeneral.hpp \
    $$PWD/../../_common/MathVector3.h \
    $$PWD/../../_common/MathVector3.hpp \
    $$PWD/../../_common/Tools.h \
    $$PWD/../../_common/Tools.hpp

HEADERS += \
    $$PWD/../SpatialFunction.h \
    $$PWD/../SpatialRingBuffer.h

SOURCES += \
    $$PWD/../SpatialFunction.cpp \
    $$PWD/../SpatialRingBuffer.cpp
