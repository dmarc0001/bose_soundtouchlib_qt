###############################################################################
####                                                                       ####
#### Versuch zu BOSE websocket API                                         ####
####                                                                       ####
###############################################################################
MAJOR                                  = 1
MINOR                                  = 0
PATCH                                  = 0
BUILD                                  = 0

# da sind die Bibiotheksdateien
EXPORT_INCLUDE                         = $${PWD}/../include
EXPORT_LIB                             = $${PWD}/../lib

win32:VERSION                          = $${MAJOR}.$${MINOR}.$${PATCH}.$${BUILD} # major.minor.patch.build
else:VERSION                           = $${MAJOR}.$${MINOR}.$${PATCH}    # major.minor.patch

DEFINES                                += QT_DEPRECATED_WARNINGS
DEFINES                                += $$DEBUG

TARGET                                 = soundtouch_qt
TEMPLATE                               = app
DESTDIR                                = ../out
LIBS                                   += -L$${EXPORT_LIB}/$${KIT}
#
QT                                     += core
QT                                     += gui
QT                                     += widgets
QT                                     += websockets
QT                                     += network
QT                                     += xml
#
CONFIG                                 += stl
CONFIG                                 += c++11

CONFIG(release, debug|release) {
  TARGET                               = soundtouch_qt
  LIBS                                 += -lsoundtouch_qt
  DEFINES                              += QT_NO_DEBUG_OUTPUT
}
CONFIG(debug, debug|release) {
  TARGET                               = soundtouch_qt_debug
  LIBS                                 += -lsoundtouch_debug_qt
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
    main.cpp \
    librarytestwindow.cpp \
    config/programconfig.cpp

HEADERS += \
    librarytestwindow.hpp \
    config/programconfig.hpp \

FORMS += \
        ui/librarytestwindow.ui




INCLUDEPATH += $${EXPORT_INCLUDE}

