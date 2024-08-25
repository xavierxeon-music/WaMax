TARGET = HelpForMax
TEMPLATE = app

QT += widgets  svg network xml
CONFIG += c++20


CONFIG(debug, debug|release) {
   DEFINES += TEST_CLIENT_AVAILABLE
}

macx {
   QMAKE_CXXFLAGS += -Werror
   ICON = $$PWD/icons/HelpForMax.icns
   #QMAKE_INFO_PLIST = Info.plist
   CONFIG(release, debug|release){
      DESTDIR = ~/Applications
      QMAKE_POST_LINK = $$(QTDIR)/bin/macdeployqt $${DESTDIR}/$${TARGET}.app
   }
   CONFIG += sdk_no_version_check
}

windows {
   QMAKE_CXXFLAGS += /WX
   RC_ICONS = $$PWD/icons/HelpForMax.ico
   CONFIG(release, debug|release) {
      DESTDIR = ../bin
      QMAKE_POST_LINK = $$(QTDIR)/bin/windeployqt --no-translations --no-system-d3d-compiler --compiler-runtime $${DESTDIR}/$${TARGET}.exe
   }
}

linux {
    QMAKE_CXXFLAGS += -Werror
    #ICON = $$PWD/icons/HelpForMax.icns
    CONFIG(release, debug|release){
       DESTDIR = ~/bin
    }
}

include(Package/Package.pri)
include(Patch/Patch.pri)

HEADERS += \
   MainWindow.h \
   MessageBar.h \
   MessageChannel.h \
   RecentFiles.h \
   SchemaWidget.h \
   TestClient.h \
   HelpForMax.h \
   HelpForMax.hpp

SOURCES += \
   MainWindow.cpp \
   MessageBar.cpp \
   MessageChannel.cpp \
   RecentFiles.cpp \
   SchemaWidget.cpp \
   TestClient.cpp

RESOURCES += \
   icons/icons.qrc

FORMS += \
    TestClient.ui
