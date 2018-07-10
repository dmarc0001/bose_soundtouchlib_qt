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

CONFIG(release, debug|release) {
  DESTDIR                              = $${PWD}/lib/$${KITNAME}/release
  DEFINES                              += QT_NO_DEBUG_OUTPUT
}
CONFIG(debug, debug|release) {
  DESTDIR                              = $${PWD}/lib/$${KITNAME}/debug
}

MOC_DIR                                = moc
RCC_DIR                                = rcc
UI_DIR                                 = ui

INCLUDEPATH                            += $${PWD}/include
INCLUDEPATH                            += $${PWD}/include/websocket
INCLUDEPATH                            += $${PWD}/include/xmlparser


unix {
    # Target auf dem entfernten GERÄT
    target.path                        = /home/pi/qt5pi/soundtouch_qt
    # Target auf lokalem Gerät
    #target.path                        = /usr/lib
    INSTALLS                           += target
}

message( library version $$VERSION )

SOURCES += \
    bsoundtouchdevice.cpp \
    xmlparser/xmlresultparser.cpp \
    xmlparser/iresponseobject.cpp \
    websocket/bwebsocket.cpp \
    xmlparser/xmlupdateparser.cpp \
    xmlparser/bsoundtouch_global.cpp \
    xmlparser/httpresponse/httppresetsobject.cpp \
    xmlparser/wscallback/wszoneupdated.cpp \
    xmlparser/wscallback/wsvolumeupdated.cpp \
    xmlparser/wscallback/wsuserinactivityupdated.cpp \
    xmlparser/wscallback/wsuseractivityupdated.cpp \
    xmlparser/wscallback/wssourceupdated.cpp \
    xmlparser/wscallback/wssoundtouchsdkinfo.cpp \
    xmlparser/wscallback/wsrecentsupdated.cpp \
    xmlparser/wscallback/wspresetupdateobject.cpp \
    xmlparser/wscallback/wsnowselectionupdated.cpp \
    xmlparser/wscallback/wsnowplayingupdate.cpp \
    xmlparser/wscallback/wsnameupdated.cpp \
    xmlparser/wscallback/wslanguageupdated.cpp \
    xmlparser/wscallback/wsinfoupdated.cpp \
    xmlparser/wscallback/wsgroupupdated.cpp \
    xmlparser/wscallback/wserrorupdated.cpp \
    xmlparser/wscallback/wsconnectionstateupdated.cpp \
    xmlparser/wscallback/wsbrowseupdated.cpp \
    xmlparser/wscallback/wsbassupdated.cpp \
    xmlparser/wscallback/wsaudiospcontrols.cpp \
    xmlparser/wscallback/wsaudioproducttonecontrols.cpp \
    xmlparser/wscallback/wsaudioproductlevelcontrols.cpp \
    xmlparser/httpresponse/httpzoneobject.cpp \
    xmlparser/httpresponse/httpvolumeobject.cpp \
    xmlparser/httpresponse/httpsourcesobject.cpp \
    xmlparser/httpresponse/httpresultokobject.cpp \
    xmlparser/httpresponse/httpresulterrorobject.cpp \
    xmlparser/httpresponse/httpnowplayingobject.cpp \
    xmlparser/httpresponse/httpgroupobject.cpp \
    xmlparser/httpresponse/httpdeviceinfoobject.cpp \
    xmlparser/httpresponse/httpbassobject.cpp \
    xmlparser/httpresponse/httpbasscapabilitiesobject.cpp

HEADERS += \
    include/xmlparser/xmlresultparser.hpp \
    include/xmlparser/iresponseobject.hpp \
    include/xmlparser/bsoundtouch_global.hpp \
    include/bsoundtouchdevice.hpp \
    include/websocket/bwebsocket.hpp \
    include/xmlparser/xmlupdateparser.hpp \
    include/xmlparser/wscallback/wssourceupdated.hpp \
    include/xmlparser/httpresponse/httppresetsobject.hpp \
    include/xmlparser/wscallback/wszoneupdated.hpp \
    include/xmlparser/wscallback/wsvolumeupdated.hpp \
    include/xmlparser/wscallback/wsuserinactivityupdated.hpp \
    include/xmlparser/wscallback/wsuseractivityupdated.hpp \
    include/xmlparser/wscallback/wssourceupdated.hpp \
    include/xmlparser/wscallback/wssoundtouchsdkinfo.hpp \
    include/xmlparser/wscallback/wsrecentsupdated.hpp \
    include/xmlparser/wscallback/wspresetupdateobject.hpp \
    include/xmlparser/wscallback/wsnowselectionupdated.hpp \
    include/xmlparser/wscallback/wsnowplayingupdate.hpp \
    include/xmlparser/wscallback/wsnameupdated.hpp \
    include/xmlparser/wscallback/wslanguageupdated.hpp \
    include/xmlparser/wscallback/wsinfoupdated.hpp \
    include/xmlparser/wscallback/wsgroupupdated.hpp \
    include/xmlparser/wscallback/wserrorupdated.hpp \
    include/xmlparser/wscallback/wsconnectionstateupdated.hpp \
    include/xmlparser/wscallback/wsbrowseupdated.hpp \
    include/xmlparser/wscallback/wsbassupdated.hpp \
    include/xmlparser/wscallback/wsaudiospcontrols.hpp \
    include/xmlparser/wscallback/wsaudioproducttonecontrols.hpp \
    include/xmlparser/wscallback/wsaudioproductlevelcontrols.hpp \
    include/xmlparser/httpresponse/httpzoneobject.hpp \
    include/xmlparser/httpresponse/httpvolumeobject.hpp \
    include/xmlparser/httpresponse/httpsourcesobject.hpp \
    include/xmlparser/httpresponse/httpresultokobject.hpp \
    include/xmlparser/httpresponse/httpresulterrorobject.hpp \
    include/xmlparser/httpresponse/httpnowplayingobject.hpp \
    include/xmlparser/httpresponse/httpgroupobject.hpp \
    include/xmlparser/httpresponse/httpdeviceinfoobject.hpp \
    include/xmlparser/httpresponse/httpbassobject.hpp \
    include/xmlparser/httpresponse/httpbasscapabilitiesobject.hpp

