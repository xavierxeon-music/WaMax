TARGET = MaxScreen
TEMPLATE = app

QT += network qml quick svg
include(../../_MobileApp.pri)
include(../../_MidiApp.pri)

CONFIG += c++20 qmltypes

QML_IMPORT_NAME = MaxScreen
QML_IMPORT_MAJOR_VERSION = 1

QMAKE_INFO_PLIST = $$PWD/Info.plist

ios_icon.files = $$files($$PWD/icons/ios/Icon-*.png)
QMAKE_BUNDLE_DATA += ios_icon

HEADERS += \
   MaxScreen.h \
   Picture.h \
   Rainbow.h


SOURCES += \
   MaxScreen.cpp \
   Picture.cpp \
   Rainbow.cpp



RESOURCES += \
   gui/MaxScreenGui.qrc \
   icons/icons.qrc


