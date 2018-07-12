###############################################################################
####                                                                       ####
#### Versuch zu BOSE websocket API                                         ####
####                                                                       ####
###############################################################################
MAJOR                                  = 1 # major version number
MINOR                                  = 0 # minor version number
PATCH                                  = 3 # patch number
BUILD                                  = 0 # win32: build number

# da hin sollen die includedateien exportiert werden
EXPORT_INCLUDE                         = $${PWD}/../include
EXPORT_LIB                             = $${PWD}/../lib

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
DESTDIR                                = $${EXPORT_LIB}/$${KIT}
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
  TARGET                               = soundtouch_qt
  DEFINES                              += QT_NO_DEBUG_OUTPUT
}
CONFIG(debug, debug|release) {
TARGET                                 = soundtouch_debug_qt
}

MOC_DIR                                = moc
RCC_DIR                                = rcc
UI_DIR                                 = ui

SOURCES += \
    bsoundtouchdevice.cpp \
    xmlparser/xmlresultparser.cpp \
    xmlparser/iresponseobject.cpp \
    websocket/bwebsocket.cpp \
    xmlparser/xmlupdateparser.cpp \
    xmlparser/bsoundtouch_global.cpp \
    xmlparser/httpresponse/httppresetsobject.cpp \
    xmlparser/httpresponse/httpzoneobject.cpp \
    xmlparser/httpresponse/httpvolumeobject.cpp \
    xmlparser/httpresponse/httpsourcesobject.cpp \
    xmlparser/httpresponse/httpresultokobject.cpp \
    xmlparser/httpresponse/httpresulterrorobject.cpp \
    xmlparser/httpresponse/httpnowplayingobject.cpp \
    xmlparser/httpresponse/httpgroupobject.cpp \
    xmlparser/httpresponse/httpdeviceinfoobject.cpp \
    xmlparser/httpresponse/httpbassobject.cpp \
    xmlparser/httpresponse/httpbasscapabilitiesobject.cpp \
    xmlparser/wscallback/wssoundtouchsdkinfo.cpp \
    xmlparser/wscallback/wsuseractivityupdated.cpp \
    xmlparser/wscallback/wszoneupdated.cpp \
    xmlparser/wscallback/wsvolumeupdated.cpp \
    xmlparser/wscallback/wsuserinactivityupdated.cpp \
    xmlparser/wscallback/wssourceupdated.cpp \
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
    xmlparser/wscallback/wsaudioproductlevelcontrols.cpp

HEADERS += \
    xmlparser/xmlresultparser.hpp \
    xmlparser/iresponseobject.hpp \
    xmlparser/bsoundtouch_global.hpp \
    bsoundtouchdevice.hpp \
    websocket/bwebsocket.hpp \
    xmlparser/xmlupdateparser.hpp \
    xmlparser/httpresponse/httppresetsobject.hpp \
    xmlparser/wscallback/wszoneupdated.hpp \
    xmlparser/wscallback/wsvolumeupdated.hpp \
    xmlparser/wscallback/wsuserinactivityupdated.hpp \
    xmlparser/wscallback/wsuseractivityupdated.hpp \
    xmlparser/wscallback/wssourceupdated.hpp \
    xmlparser/wscallback/wssoundtouchsdkinfo.hpp \
    xmlparser/wscallback/wsrecentsupdated.hpp \
    xmlparser/wscallback/wspresetupdateobject.hpp \
    xmlparser/wscallback/wsnowselectionupdated.hpp \
    xmlparser/wscallback/wsnowplayingupdate.hpp \
    xmlparser/wscallback/wsnameupdated.hpp \
    xmlparser/wscallback/wslanguageupdated.hpp \
    xmlparser/wscallback/wsinfoupdated.hpp \
    xmlparser/wscallback/wsgroupupdated.hpp \
    xmlparser/wscallback/wserrorupdated.hpp \
    xmlparser/wscallback/wsconnectionstateupdated.hpp \
    xmlparser/wscallback/wsbrowseupdated.hpp \
    xmlparser/wscallback/wsbassupdated.hpp \
    xmlparser/wscallback/wsaudiospcontrols.hpp \
    xmlparser/wscallback/wsaudioproducttonecontrols.hpp \
    xmlparser/wscallback/wsaudioproductlevelcontrols.hpp \
    xmlparser/httpresponse/httpzoneobject.hpp \
    xmlparser/httpresponse/httpvolumeobject.hpp \
    xmlparser/httpresponse/httpsourcesobject.hpp \
    xmlparser/httpresponse/httpresultokobject.hpp \
    xmlparser/httpresponse/httpresulterrorobject.hpp \
    xmlparser/httpresponse/httpnowplayingobject.hpp \
    xmlparser/httpresponse/httpgroupobject.hpp \
    xmlparser/httpresponse/httpdeviceinfoobject.hpp \
    xmlparser/httpresponse/httpbassobject.hpp \
    xmlparser/httpresponse/httpbasscapabilitiesobject.hpp

#
# extra targets einfügen (für Makefile)
#
headercopy.target                     = headers
headercopy.commands                   = cd $${PWD} && cp -f --parents $${HEADERS} $${EXPORT_INCLUDE}
headercopy.depends                    = first

headerdel.target                      = delheaders
headerdel.commands                    = cd $${EXPORT_INCLUDE} && rm -rf *

QMAKE_EXTRA_TARGETS += headercopy
QMAKE_EXTRA_TARGETS += headerdel


message( library version $$VERSION kit $${KIT} )
