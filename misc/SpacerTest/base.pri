MAX_OBJECTS = ../../source/maxobjects

INCLUDEPATH += \
    $$MAX_OBJECTS/wa.spacer_tilde \
    $$MAX_OBJECTS/_common

HEADERS += \
    $$MAX_OBJECTS/_common/MathGeneral.h \
    $$MAX_OBJECTS/_common/MathGeneral.hpp \
    $$MAX_OBJECTS/_common/MathVector3.h \
    $$MAX_OBJECTS/_common/MathVector3.hpp \
    $$MAX_OBJECTS/_common/Tools.h \
    $$MAX_OBJECTS/_common/Tools.hpp

HEADERS += \
    $$MAX_OBJECTS/wa.spacer_tilde/SpatialCoords.h \
    $$MAX_OBJECTS/wa.spacer_tilde/SpatialFunction.h \
    $$MAX_OBJECTS/wa.spacer_tilde/SpatialRingBuffer.h

SOURCES += \
    $$MAX_OBJECTS/wa.spacer_tilde/SpatialFunction.cpp \
    $$MAX_OBJECTS/wa.spacer_tilde/SpatialRingBuffer.cpp
