QT = core network

include($$PWD/qtzeroconf.pri)

#VERSION = 1.1

TEMPLATE = lib
#TARGET = $$qtLibraryTarget(QtZeroConf$$QT_LIBINFIX)
CONFIG += module create_prl
DEFINES+= QT_BUILD_ZEROCONF_LIB
mac:QMAKE_FRAMEWORK_BUNDLE_NAME = $$TARGET

linux:!android {
  CONFIG(debug, debug|release) {
    TARGET = $$qtLibraryTarget(QtZeroConfd$$QT_LIBINFIX)
    message(LINUX DEBUG library:  $$TARGET )
  }
  else {
    TARGET = $$qtLibraryTarget(QtZeroConf$$QT_LIBINFIX)
    message(LINUX no DEBUG library:  $$TARGET )
  }
}
else {
  TARGET = $$qtLibraryTarget(QtZeroConf$$QT_LIBINFIX)
  message(OTHER library:  $$TARGET )
}


headersDataFiles.files = $$PWD/qzeroconf.h $$PWD/qzeroconfservice.h $$PWD/qzeroconfglobal.h

# install to Qt installation directory if no PREFIX specified
_PREFIX = $$PREFIX
isEmpty(_PREFIX) {
	INSTALL_HEADER_PATH = $$[QT_INSTALL_HEADERS]/QtZeroConf/
	INSTALL_LIB_PATH = $$[QT_INSTALL_LIBS]
} else {

	INSTALL_HEADER_PATH = $$PREFIX/include/QtZeroConf/
	INSTALL_LIB_PATH = $$PREFIX/lib
}
message(install to: $$INSTALL_LIB_PATH)
headersDataFiles.path = $$INSTALL_HEADER_PATH
target.path = $$INSTALL_LIB_PATH

INSTALLS+= target headersDataFiles


#linux:!android {
#	HEADERS+= $$PWD/qzeroconf.h $$PWD/avahi-qt/qt-watch.h  $$PWD/avahi-qt/qt-watch_p.h
#	SOURCES+= $$PWD/avahiclient.cpp $$PWD/avahi-qt/qt-watch.cpp
#	LIBS+= -lavahi-client -lavahi-common
#	QMAKE_CXXFLAGS+= -I$$PWD
#}
