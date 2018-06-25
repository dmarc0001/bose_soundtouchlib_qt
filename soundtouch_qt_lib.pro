###############################################################################
####                                                                       ####
#### Versuch zu BOSE websocket API                                         ####
####                                                                       ####
###############################################################################

TARGET                                 = libsoundtouch_qt
TEMPLATE                               = lib
#
QT                                     += core
QT                                     -= gui
QT                                     += websockets
QT                                     += network
QT                                     += xml

CONFIG                                 += stl
CONFIG                                 += c++11
#DESTDIR                                = ../out
CONFIG(release, debug|release) {
  DESTDIR                              = ../rout
}
CONFIG(debug, debug|release) {
  DESTDIR                              = ../dout
}

MOC_DIR                                = moc
RCC_DIR                                = rcc
UI_DIR                                 = ui


unix {
    # Target auf dem entfernten GERÄT
    target.path                        = /home/pi/qt5pi/soundtouch_qt
    # Target auf lokalem Gerät
    #target.path                        = /usr/lib
    INSTALLS                           += target
}


DEFINES                                += SOUNDTOUCH_QT_LIB_LIBRARY
DEFINES                                += QT_DEPRECATED_WARNINGS

DEFINES                                += $$DEBUG

SOURCES += \
    libsrc/logging/Logger.cpp \
    libsrc/bsoundtouchdevice.cpp \
    libsrc/xmlparser/xmlresultparser.cpp \
    libsrc/xmlparser/deviceinfoobject.cpp \
    libsrc/xmlparser/iresponseobject.cpp \
    libsrc/xmlparser/nowplayingobject.cpp \
    libsrc/xmlparser/volumeobject.cpp \
    libsrc/xmlparser/sourcesobject.cpp \
    libsrc/xmlparser/zoneobject.cpp \
    libsrc/xmlparser/bassobject.cpp \
    libsrc/xmlparser/basscapabilitiesobject.cpp \
    libsrc/xmlparser/presetsobject.cpp \
    libsrc/xmlparser/groupobject.cpp \
    libsrc/xmlparser/resultokobject.cpp \
    libsrc/xmlparser/resulterrorobject.cpp \
    libsrc/websocket/bwebsocket.cpp

HEADERS += \
    libsrc/logging/Logger.hpp \
    libsrc/xmlparser/xmlresultparser.hpp \
    libsrc/xmlparser/deviceinfoobject.hpp \
    libsrc/xmlparser/iresponseobject.hpp \
    libsrc/xmlparser/nowplayingobject.hpp \
    libsrc/xmlparser/volumeobject.hpp \
    libsrc/xmlparser/sourcesobject.hpp \
    libsrc/xmlparser/zoneobject.hpp \
    libsrc/xmlparser/bsoundtouch_global.hpp \
    libsrc/xmlparser/bassobject.hpp \
    libsrc/xmlparser/basscapabilitiesobject.hpp \
    libsrc/xmlparser/presetsobject.hpp \
    libsrc/xmlparser/groupobject.hpp \
    libsrc/xmlparser/resultokobject.hpp \
    libsrc/xmlparser/resulterrorobject.hpp \
    libsrc/bsoundtouchdevice.hpp \
    libsrc/websocket/bwebsocket.hpp

