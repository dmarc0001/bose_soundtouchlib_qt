###############################################################################
####                                                                       ####
#### Versuch zu BOSE websocket API                                         ####
####                                                                       ####
###############################################################################
MAJOR                                  = 1 # major version number
MINOR                                  = 0 # minor version number
PATCH                                  = 2 # patch number
BUILD                                  = 0 # win32: build number

$$MAJOR.$$MINOR.$$PATCH
win32:VERSION                          = $${MAJOR}.$${MINOR}.$${PATCH}.$${BUILD} # major.minor.patch.build
else:VERSION                           = $${MAJOR}.$${MINOR}.$${PATCH}    # major.minor.patch

DEFINES                                += SOUNDTOUCH_QT_LIB_LIBRARY
DEFINES                                += QT_DEPRECATED_WARNINGS
DEFINES                                += $$DEBUG
DEFINES                                += VMAJOR=$$MAJOR
DEFINES                                += VMINOR=$$MINOR
DEFINES                                += VPATCH=$$PATCH

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

message( library version $$VERSION )

SOURCES += \
    libsrc/bsoundtouchdevice.cpp \
    libsrc/xmlparser/xmlresultparser.cpp \
    libsrc/xmlparser/iresponseobject.cpp \
    libsrc/websocket/bwebsocket.cpp \
    libsrc/xmlparser/xmlupdateparser.cpp \
    libsrc/xmlparser/bsoundtouch_global.cpp \
    libsrc/xmlparser/httpresponse/httppresetsobject.cpp \
    libsrc/xmlparser/wscallback/wszoneupdated.cpp \
    libsrc/xmlparser/wscallback/wsvolumeupdated.cpp \
    libsrc/xmlparser/wscallback/wsuserinactivityupdated.cpp \
    libsrc/xmlparser/wscallback/wsuseractivityupdated.cpp \
    libsrc/xmlparser/wscallback/wssourceupdated.cpp \
    libsrc/xmlparser/wscallback/wssoundtouchsdkinfo.cpp \
    libsrc/xmlparser/wscallback/wsrecentsupdated.cpp \
    libsrc/xmlparser/wscallback/wspresetupdateobject.cpp \
    libsrc/xmlparser/wscallback/wsnowselectionupdated.cpp \
    libsrc/xmlparser/wscallback/wsnowplayingupdate.cpp \
    libsrc/xmlparser/wscallback/wsnameupdated.cpp \
    libsrc/xmlparser/wscallback/wslanguageupdated.cpp \
    libsrc/xmlparser/wscallback/wsinfoupdated.cpp \
    libsrc/xmlparser/wscallback/wsgroupupdated.cpp \
    libsrc/xmlparser/wscallback/wserrorupdated.cpp \
    libsrc/xmlparser/wscallback/wsconnectionstateupdated.cpp \
    libsrc/xmlparser/wscallback/wsbrowseupdated.cpp \
    libsrc/xmlparser/wscallback/wsbassupdated.cpp \
    libsrc/xmlparser/wscallback/wsaudiospcontrols.cpp \
    libsrc/xmlparser/wscallback/wsaudioproducttonecontrols.cpp \
    libsrc/xmlparser/wscallback/wsaudioproductlevelcontrols.cpp \
    libsrc/xmlparser/httpresponse/httpzoneobject.cpp \
    libsrc/xmlparser/httpresponse/httpvolumeobject.cpp \
    libsrc/xmlparser/httpresponse/httpsourcesobject.cpp \
    libsrc/xmlparser/httpresponse/httpresultokobject.cpp \
    libsrc/xmlparser/httpresponse/httpresulterrorobject.cpp \
    libsrc/xmlparser/httpresponse/httpnowplayingobject.cpp \
    libsrc/xmlparser/httpresponse/httpgroupobject.cpp \
    libsrc/xmlparser/httpresponse/httpdeviceinfoobject.cpp \
    libsrc/xmlparser/httpresponse/httpbassobject.cpp \
    libsrc/xmlparser/httpresponse/httpbasscapabilitiesobject.cpp

HEADERS += \
    libsrc/xmlparser/xmlresultparser.hpp \
    libsrc/xmlparser/iresponseobject.hpp \
    libsrc/xmlparser/bsoundtouch_global.hpp \
    libsrc/bsoundtouchdevice.hpp \
    libsrc/websocket/bwebsocket.hpp \
    libsrc/xmlparser/xmlupdateparser.hpp \
    libsrc/xmlparser/wscallback/wssourceupdated.hpp \
    libsrc/xmlparser/httpresponse/httppresetsobject.hpp \
    libsrc/xmlparser/wscallback/wszoneupdated.hpp \
    libsrc/xmlparser/wscallback/wsvolumeupdated.hpp \
    libsrc/xmlparser/wscallback/wsuserinactivityupdated.hpp \
    libsrc/xmlparser/wscallback/wsuseractivityupdated.hpp \
    libsrc/xmlparser/wscallback/wssourceupdated.hpp \
    libsrc/xmlparser/wscallback/wssoundtouchsdkinfo.hpp \
    libsrc/xmlparser/wscallback/wsrecentsupdated.hpp \
    libsrc/xmlparser/wscallback/wspresetupdateobject.hpp \
    libsrc/xmlparser/wscallback/wsnowselectionupdated.hpp \
    libsrc/xmlparser/wscallback/wsnowplayingupdate.hpp \
    libsrc/xmlparser/wscallback/wsnameupdated.hpp \
    libsrc/xmlparser/wscallback/wslanguageupdated.hpp \
    libsrc/xmlparser/wscallback/wsinfoupdated.hpp \
    libsrc/xmlparser/wscallback/wsgroupupdated.hpp \
    libsrc/xmlparser/wscallback/wserrorupdated.hpp \
    libsrc/xmlparser/wscallback/wsconnectionstateupdated.hpp \
    libsrc/xmlparser/wscallback/wsbrowseupdated.hpp \
    libsrc/xmlparser/wscallback/wsbassupdated.hpp \
    libsrc/xmlparser/wscallback/wsaudiospcontrols.hpp \
    libsrc/xmlparser/wscallback/wsaudioproducttonecontrols.hpp \
    libsrc/xmlparser/wscallback/wsaudioproductlevelcontrols.hpp \
    libsrc/xmlparser/httpresponse/httpzoneobject.hpp \
    libsrc/xmlparser/httpresponse/httpvolumeobject.hpp \
    libsrc/xmlparser/httpresponse/httpsourcesobject.hpp \
    libsrc/xmlparser/httpresponse/httpresultokobject.hpp \
    libsrc/xmlparser/httpresponse/httpresulterrorobject.hpp \
    libsrc/xmlparser/httpresponse/httpnowplayingobject.hpp \
    libsrc/xmlparser/httpresponse/httpgroupobject.hpp \
    libsrc/xmlparser/httpresponse/httpdeviceinfoobject.hpp \
    libsrc/xmlparser/httpresponse/httpbassobject.hpp \
    libsrc/xmlparser/httpresponse/httpbasscapabilitiesobject.hpp

