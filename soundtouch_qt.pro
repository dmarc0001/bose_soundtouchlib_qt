###############################################################################
####                                                                       ####
#### Versuch zu BOSE websocket API                                         ####
####                                                                       ####
###############################################################################
MAJOR                                  = 1
MINOR                                  = 0
PATCH                                  = 0
BUILD                                  = 0

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
  LIBS                                 += -L$$OUT_PWD/../rout -llibsoundtouch_qt
}
CONFIG(debug, debug|release) {
  DESTDIR                              = ../dout
  LIBS                                 += -L$$OUT_PWD/../dout -llibsoundtouch_qt
}

MOC_DIR                                = moc
RCC_DIR                                = rcc
UI_DIR                                 = ui

#
# Target auf dem entfernten GERÄT
#
target.path                            = /home/pi/qt5pi/soundtouch_qt
INSTALLS                               += target

SOURCES += \
    src/main.cpp \
    src/librarytestwindow.cpp \
    src/config/programconfig.cpp

HEADERS += \
    src/librarytestwindow.hpp \
    src/config/programconfig.hpp \

FORMS += \
        ui/librarytestwindow.ui


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/./release/ -llibsoundtouch_qt
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/./debug/ -llibsoundtouch_qt
else:unix: LIBS += -L$$PWD/./lib/debug -llibsoundtouch_qt




INCLUDEPATH += $$PWD/libsrc
DEPENDPATH += $$PWD/libsrc

