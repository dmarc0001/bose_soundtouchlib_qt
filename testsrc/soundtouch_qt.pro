###############################################################################
####                                                                       ####
#### Versuch zu BOSE websocket API                                         ####
####                                                                       ####
###############################################################################
MAJOR                                  = 1
MINOR                                  = 0
PATCH                                  = 0
BUILD                                  = 0
LIBRARYPATH                            = $${PWD}/../libsrc

$$MAJOR.$$MINOR.$$PATCH
win32:VERSION                          = $$MAJOR.$$MINOR.$$PATCH.$$BUILD # major.minor.patch.build
else:VERSION                           = $$MAJOR.$$MINOR.$$PATCH    # major.minor.patch

DEFINES                                += QT_DEPRECATED_WARNINGS
DEFINES                                += $$DEBUG

TARGET                                 = soundtouch_qt
TEMPLATE                               = app
#
QT                                     += core
QT                                     += gui
QT                                     += widgets
QT                                     += websockets
QT                                     += network
QT                                     += xml

CONFIG                                 += stl
CONFIG                                 += c++11

#DESTDIR                                = ../out
CONFIG(release, debug|release) {
  DESTDIR                              = ../rout
  LIBS                                 += -L$${LIBRARYPATH}/lib/$${KITNAME}/release
  win32:LIBS                           += -llibsoundtouch_qt$${MAJOR}
  else:LIBS                            += -llibsoundtouch_qt
  DEFINES                              += QT_NO_DEBUG_OUTPUT
}
CONFIG(debug, debug|release) {
  DESTDIR                              = ../dout
  LIBS                                 += -L$${LIBRARYPATH}/lib/$${KITNAME}/debug
  win32:LIBS                           += -llibsoundtouch_qt$${MAJOR}
  else:LIBS                            += -llibsoundtouch_qt
}

MOC_DIR                                = moc
RCC_DIR                                = rcc
UI_DIR                                 = ui

INCLUDEPATH                            += $${LIBRARYPATH}/include
INCLUDEPATH                            += $${LIBRARYPATH}/include/websocket
INCLUDEPATH                            += $${LIBRARYPATH}/include/xmlparser

message( kit filename - $${KITNAME} $${LIBRARYPATH})

#
# Target auf dem entfernten GERÄT
#
target.path                            = /home/pi/qt5pi/soundtouch_qt
INSTALLS                               += target

SOURCES += \
    main.cpp \
    librarytestwindow.cpp \
    config/programconfig.cpp

HEADERS += \
    librarytestwindow.hpp \
    config/programconfig.hpp \

FORMS += \
        ui/librarytestwindow.ui


#win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/./release/ -llibsoundtouch_qt
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/./debug/ -llibsoundtouch_qt
#else:unix: LIBS += -L$$PWD/./lib/debug -llibsoundtouch_qt

