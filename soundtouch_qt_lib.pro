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
    libsrc/xmlparser/wspresetupdateobject.cpp \
    libsrc/xmlparser/wssoundtouchsdkinfo.cpp \
    libsrc/xmlparser/xmlupdateparser.cpp \
    libsrc/xmlparser/bsoundtouch_global.cpp \
    libsrc/xmlparser/wsnowplayingupdate.cpp \
    libsrc/xmlparser/wsnowselectionupdated.cpp \
    libsrc/xmlparser/wsvolumeupdated.cpp \
    libsrc/xmlparser/wsbassupdated.cpp \
    libsrc/xmlparser/wszoneupdated.cpp \
    libsrc/xmlparser/wsinfoupdated.cpp \
    libsrc/xmlparser/wsnameupdated.cpp \
    libsrc/xmlparser/wserrorupdated.cpp \
    libsrc/xmlparser/wsgroupupdated.cpp \
    libsrc/xmlparser/wsbrowseupdated.cpp \
    libsrc/xmlparser/wsrecentsupdated.cpp \
    libsrc/xmlparser/wssourceupdated.cpp \
    libsrc/xmlparser/wslanguageupdated.cpp \
    libsrc/xmlparser/wsuseractivityupdated.cpp \
    libsrc/xmlparser/wsuserinactivityupdated.cpp \
    libsrc/xmlparser/wsconnectionstateupdated.cpp \
    libsrc/xmlparser/wsaudioproducttonecontrols.cpp \
    libsrc/xmlparser/wsaudioproductlevelcontrols.cpp \
    libsrc/xmlparser/wsaudiospcontrols.cpp

HEADERS += \
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
    libsrc/xmlparser/wssoundtouchsdkinfo.hpp \
    libsrc/xmlparser/xmlupdateparser.hpp \
    libsrc/xmlparser/wsnowplayingupdate.hpp \
    libsrc/xmlparser/wsnowselectionupdated.hpp \
    libsrc/xmlparser/wsvolumeupdated.hpp \
    libsrc/xmlparser/wsbassupdated.hpp \
    libsrc/xmlparser/wszoneupdated.hpp \
    libsrc/xmlparser/wsinfoupdated.hpp \
    libsrc/xmlparser/wsnameupdated.hpp \
    libsrc/xmlparser/wserrorupdated.hpp \
    libsrc/xmlparser/wsgroupupdated.hpp \
    libsrc/xmlparser/wsbrowseupdated.hpp \
    libsrc/xmlparser/wsrecentsupdated.hpp \
    libsrc/xmlparser/wssourceupdated.hpp \
    libsrc/xmlparser/wslanguageupdated.hpp \
    libsrc/xmlparser/wsuseractivityupdated.hpp \
    libsrc/xmlparser/wsuserinactivityupdated.hpp \
    libsrc/xmlparser/wsconnectionstateupdated.hpp \
    libsrc/xmlparser/wsaudioproducttonecontrols.hpp \
    libsrc/xmlparser/wsaudioproductlevelcontrols.hpp \
    libsrc/xmlparser/wsaudiospcontrols.hpp

