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
    libsrc/xmlparser/iresponseobject.cpp \
    libsrc/websocket/bwebsocket.cpp \
    libsrc/xmlparser/httpbasscapabilitiesobject.cpp \
    libsrc/xmlparser/httpbassobject.cpp \
    libsrc/xmlparser/httpdeviceinfoobject.cpp \
    libsrc/xmlparser/httpgroupobject.cpp \
    libsrc/xmlparser/httppresetsobject.cpp \
    libsrc/xmlparser/httpnowplayingobject.cpp \
    libsrc/xmlparser/httpresulterrorobject.cpp \
    libsrc/xmlparser/httpresultokobject.cpp \
    libsrc/xmlparser/httpsourcesobject.cpp \
    libsrc/xmlparser/httpvolumeobject.cpp \
    libsrc/xmlparser/httpzoneobject.cpp \
    libsrc/xmlparser/wspresetupdateobject.cpp

HEADERS += \
    libsrc/logging/Logger.hpp \
    libsrc/xmlparser/xmlresultparser.hpp \
    libsrc/xmlparser/iresponseobject.hpp \
    libsrc/xmlparser/bsoundtouch_global.hpp \
    libsrc/bsoundtouchdevice.hpp \
    libsrc/websocket/bwebsocket.hpp \
    libsrc/xmlparser/httpbasscapabilitiesobject.hpp \
    libsrc/xmlparser/httpbassobject.hpp \
    libsrc/xmlparser/httpdeviceinfoobject.hpp \
    libsrc/xmlparser/httpgroupobject.hpp \
    libsrc/xmlparser/httppresetsobject.hpp \
    libsrc/xmlparser/httpnowplayingobject.hpp \
    libsrc/xmlparser/httpresulterrorobject.hpp \
    libsrc/xmlparser/httpresultokobject.hpp \
    libsrc/xmlparser/httpsourcesobject.hpp \
    libsrc/xmlparser/httpvolumeobject.hpp \
    libsrc/xmlparser/httpzoneobject.hpp \
    libsrc/xmlparser/wspresetupdateobject.hpp \
    libsrc/xmlparser/soundtouch_response.hpp

