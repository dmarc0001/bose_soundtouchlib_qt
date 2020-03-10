cmake_minimum_required(VERSION 3.5)

project(alert_daemon LANGUAGES CXX)

unset(ZC_EXTRA_LIBS)
unset(ZEROCONF_LIB)
unset(ZC_EXTRA_INCLUDE)
unset(EXTERNAL_LIBS)

###############################################################################
# crosscompiler Sachen, falls nötig                                           #
###############################################################################
if (CMAKE_CROSSCOMPILING)
    message("############### SOUNDTOUCH ALERT DAEMON CROSS COMPILER ###################")
    set(ZC_EXTRA_INCLUDE qtzeroconf)
else()
    message("############### SOUNDTOUCH ALERT DAEMON NATIVE COMPILER ###################")
endif()

###############################################################################
#                                                                             #
# compiler Definitionen                                                       #
#                                                                             #
###############################################################################

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

###############################################################################
# Projekt definitionen                                                        #
###############################################################################
set(KIT QT5141)
set(MAJOR 0) # major version number
set(MINOR 2) # minor version number
set(PATCH 0) # patch number
set(BUILD 0) # win32: build number
if (WIN32)
  # major.minor.patch.build
  set(VERSION $${MAJOR}.$${MINOR}.$${PATCH}.$${BUILD})
else(WIN32)
  # major.minor.patch
  set(VERSION $${MAJOR}.$${MINOR}.$${PATCH})
endif(WIN32)

if(UNIX)
  add_compile_definitions(UNIX)
  set(ZC_EXTRA_LIBS avahi-client avahi-common)
endif(UNIX)

if(WIN32)
  if (CMAKE_BUILD_TYPE  STREQUAL "Debug")
    add_compile_definitions(WIN32)
  endif()
endif(WIN32)


###############################################################################
# Einstellungen für die soundtouch library                                    #
###############################################################################
set(IMPORT_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../libsoundtouch")
include_directories(${IMPORT_INCLUDE_DIR} ${ZC_EXTRA_INCLUDE})
set(IMPORT_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../lib")

###############################################################################
# DEBUG oder kein DEBUG                                                       #
###############################################################################

if (CMAKE_BUILD_TYPE  STREQUAL "Debug")
  set(prog_name alert_daemon_debug)
  set(lib_name soundtouch_debug)
  message("create ${prog_name} in debug mode (${CMAKE_BUILD_TYPE})")
  add_compile_definitions(DEBUG)
  if (WIN32)
    set(ZEROCONF_LIB QtZeroConfd )
  endif(WIN32)
else()
  set(prog_name alert_daemon)
  set(lib_name soundtouch)
  message("create  ${prog_name} in release mode (${CMAKE_BUILD_TYPE})")
  add_compile_definitions(QT_NO_DEBUG_OUTPUT)
  if (WIN32)
    set(ZEROCONF_LIB QtZeroConf )
  endif(WIN32)
endif()

if(WIN32)
  message("ZEROCONF Lib : ${ZEROCONF_LIB}")
  message("ZEROCONF Path: ${ZEROCONF_LIB_DIR}")
endif(WIN32)

###############################################################################
# Defines für den Compiler                                                    #
###############################################################################
set(qt_defs SOUNDTOUCH_QT_LIB_IMPORT QT_DEPRECATED_WARNINGS)
set(prj_version VMAJOR=${MAJOR} VMINOR=${MINOR} VPATCH=${PATCH})
add_compile_definitions(${qt_defs} ${prj_version})

###############################################################################
# librarys finden                                                             #
###############################################################################
find_package(Qt5 COMPONENTS Core WebSockets Network Xml REQUIRED)

if (CMAKE_CROSSCOMPILING)
    link_directories( ${IMPORT_LIB_DIR}
        /home/dmarcini/raspi/sysroot/usr/lib/arm-linux-gnueabihf
        /home/dmarcini/raspi/sysroot/lib/arm-linux-gnueabihf
        )
    set( AVAHILIB
        avahi-client
        avahi-common
        dbus-1
        lzma
        systemd
        lz4
        /home/dmarcini/raspi/sysroot/lib/arm-linux-gnueabihf/libgcrypt.so.20
        /home/dmarcini/raspi/sysroot/lib/arm-linux-gnueabihf/libgpg-error.so.0
       )
    set( EXTERNAL_LIBS ${AVAHILIB} ${LIBSYSTEMD})
else(CMAKE_CROSSCOMPILING)
  if(WIN32)
    set( EXTERNAL_LIBS ${ZEROCONF_LIB})
    link_directories(${IMPORT_LIB_DIR})
  else(WIN32)
    set( EXTERNAL_LIBS ${ZC_EXTRA_LIBS})
    link_directories(${IMPORT_LIB_DIR})
  endif(WIN32)
endif()
set(IMPORT_LIBS ${lib_name} ${ZEROCONF_LIB} )
set(QT_CORELIBS Qt5::Core Qt5::Network Qt5::WebSockets Qt5::Xml )

###############################################################################
# die Quellen des Programms definieren                                        #
###############################################################################

if(WIN32)
  #
else(WIN32)
  set (HEADERS_Z
    qtzeroconf/qzeroconf.h
    qtzeroconf/avahi-qt/qt-watch.h
    qtzeroconf/avahi-qt/qt-watch_p.h
    qtzeroconf/qzeroconfservice.h
    qtzeroconf/qzeroconfglobal.h
    )
endif(WIN32)

set( HEADERS
  main.hpp
  ${HEADERS_Z}
  config/alertconfig.hpp
  config/common_def.hpp
  config/soundtouchdevice.hpp
  logging/logger.hpp
  support/nologgerexception.hpp
  discover/soundtouchdiscover.hpp
  daemon/bosecommserver.hpp
  daemon/connectionhandler.hpp
  daemon/commandgethandler.hpp
  daemon/commandsethandler.hpp
  daemon/daemontimer.hpp
  daemon/bosesoundalert.hpp
  ${EXTRA_ZC_HEADERS}
)

if(WIN32)
  #
else(WIN32)
  set( SOURCES_Z
    qtzeroconf/avahiclient.cpp
    qtzeroconf/avahi-qt/qt-watch.cpp
    qtzeroconf/qzeroconfservice.cpp
    )
endif(WIN32)


set( SOURCES
  main.cpp
  ${SOURCES_Z}
  config/alertconfig.cpp
  config/common_def.cpp
  config/soundtouchdevice.cpp
  logging/logger.cpp
  discover/soundtouchdiscover.cpp
  daemon/bosecommserver.cpp
  daemon/connectionhandler.cpp
  daemon/commandgethandler.cpp
  daemon/commandsethandler.cpp
  daemon/daemontimer.cpp
  daemon/bosesoundalert.cpp
  ${EXTRA_ZC_SOURCES}
)

add_executable( ${prog_name}
  ${HEADERS}
  ${SOURCES}
)

# und zusammenfassen zum Programm
target_link_libraries(${prog_name} PRIVATE ${QT_CORELIBS} ${IMPORT_LIBS} ${EXTERNAL_LIBS})




