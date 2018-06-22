###############################################################################
####                                                                       ####
#### Versuch zu BOSE websocket API                                         ####
####                                                                       ####
###############################################################################

TARGET                                 = soundtouch_qt
TEMPLATE                               = lib
#
QT                                     += core
QT                                     -= gui
QT                                     += websockets
QT                                     += network
QT                                     += xml

CONFIG                                 += stl
CONFIG                                 += c++11

DESTDIR                                = out
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
    src/logging/Logger.cpp \
    src/bsoundtouchdevice.cpp \
    src/config/programconfig.cpp \
    src/xmlparser/xmlresultparser.cpp \
    src/xmlparser/deviceinfoobject.cpp \
    src/xmlparser/iresponseobject.cpp \
    src/xmlparser/nowplayingobject.cpp \
    src/xmlparser/volumeobject.cpp \
    src/xmlparser/sourcesobject.cpp \
    src/xmlparser/zoneobject.cpp \
    src/xmlparser/bassobject.cpp \
    src/xmlparser/basscapabilitiesobject.cpp \
    src/xmlparser/presetsobject.cpp \
    src/xmlparser/groupobject.cpp \
    src/xmlparser/resultokobject.cpp \
    src/xmlparser/resulterrorobject.cpp

HEADERS += \
    src/logging/Logger.hpp \
    src/xmlparser/xmlresultparser.hpp \
    src/xmlparser/deviceinfoobject.hpp \
    src/xmlparser/iresponseobject.hpp \
    src/xmlparser/nowplayingobject.hpp \
    src/xmlparser/volumeobject.hpp \
    src/xmlparser/sourcesobject.hpp \
    src/xmlparser/zoneobject.hpp \
    src/xmlparser/bsoundtouch_global.hpp \
    src/xmlparser/bassobject.hpp \
    src/xmlparser/basscapabilitiesobject.hpp \
    src/xmlparser/presetsobject.hpp \
    src/xmlparser/groupobject.hpp \
    src/xmlparser/resultokobject.hpp \
    src/xmlparser/resulterrorobject.hpp \
    src/bsoundtouchdevice.hpp
