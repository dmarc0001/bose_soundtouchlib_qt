###############################################################################
####                                                                       ####
#### Versuch zu BOSE websocket API                                         ####
####                                                                       ####
###############################################################################

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

DESTDIR                                = out
MOC_DIR                                = moc
RCC_DIR                                = rcc
UI_DIR                                 = ui

#
# Target auf dem entfernten GERÄT
#
target.path                            = /home/pi/qt5pi/soundtouch_qt
INSTALLS                               += target


DEFINES                                += QT_DEPRECATED_WARNINGS
DEFINES                                += $$DEBUG

SOURCES += \
    src/main.cpp \
    src/logging/Logger.cpp \
    src/librarytestwindow.cpp \
    src/bsoundtouchdevice.cpp \
    src/config/programconfig.cpp \
    src/xmlparser/xmlresultparser.cpp \
    src/xmlparser/deviceinfoobject.cpp \
    src/xmlparser/iresponseobject.cpp \
    src/xmlparser/nowplayingobject.cpp \
    src/xmlparser/volumeobject.cpp \
    src/xmlparser/sourcesobject.cpp \
    src/xmlparser/zoneobject.cpp

HEADERS += \
    src/logging/Logger.hpp \
    src/librarytestwindow.hpp \
    src/bsoundtouchdevice.hpp \
    src/config/programconfig.hpp \
    src/xmlparser/xmlresultparser.hpp \
    src/xmlparser/deviceinfoobject.hpp \
    src/xmlparser/iresponseobject.hpp \
    src/xmlparser/nowplayingobject.hpp \
    src/xmlparser/volumeobject.hpp \
    src/xmlparser/sourcesobject.hpp \
    src/xmlparser/zoneobject.hpp \
    src/global/bsoundtouch_global.hpp

FORMS += \
        ui/librarytestwindow.ui

