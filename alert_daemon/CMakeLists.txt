cmake_minimum_required(VERSION 3.5)

project(alert_daemon LANGUAGES CXX)

###############################################################################
# crosscompiler Sachen, falls nötig                                           #
###############################################################################
if (CMAKE_CROSSCOMPILING)
    message("############### SOUNDTOUCH ALERT DAEMON CROSS COMPILER ###################")
    set(EXTRA_ZC_INCLUDE qtzeroconf)
    # im cmake includefile set(QTDIR /home/dmarcini/raspi/qt-5-14-1-ext)
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
else()
  # major.minor.patch
  set(VERSION $${MAJOR}.$${MINOR}.$${PATCH})
endif()

if(UNIX)
  add_compile_definitions(UNIX)
  set(EXTRA_ZC_LIBS avahi-client avahi-common)
endif(UNIX)
if(WIN32)
    add_compile_definitions(
        WIN32
        NDEBUG
        _WINDOWS
        _USRDLL
        MDNS_DEBUGMSGS=0
        WIN32_LEAN_AND_MEAN
        USE_TCP_LOOPBACK
        _CRT_SECURE_NO_DEPRECATE
        _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES=1
        NOT_HAVE_SA_LEN
        )
    set(EXTRA_ZC_INCLUDE qtzeroconf qtzeroconf/bonjour-sdk)
    set(EXTRA_ZC_HEADERS
        qtzeroconf/bonjour_p.h)
    set(EXTRA_ZC_SOURCES
        qtzeroconf/bonjour.cpp
        qtzeroconf/bonjour-sdk/dnssd_clientlib.c
        qtzeroconf/bonjour-sdk/dnssd_clientstub.c
        qtzeroconf//bonjour-sdk/dnssd_ipc.c
        )
    set(EXTRA_ZC_LIBS ws2_32 wsock32)
endif(WIN32)

###############################################################################
# Einstellungen für die soundtouch library                                    #
###############################################################################
set(IMPORT_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/../libsoundtouch")
include_directories(${IMPORT_INCLUDE} ${EXTRA_ZC_INCLUDE})
set(IMPORT_LIB "${CMAKE_CURRENT_SOURCE_DIR}/../lib")

###############################################################################
# DEBUG oder kein DEBUG                                                       #
###############################################################################
if (CMAKE_BUILD_TYPE  STREQUAL "Debug")
  set(prog_name alert_daemon_debug)
  set(lib_name soundtouch_debug)
  message("create ${prog_name} in debug mode (${CMAKE_BUILD_TYPE})")
  add_compile_definitions(DEBUG)
else()
  set(prog_name alert_daemon)
  set(lib_name soundtouch)
  message("create  ${prog_name} in release mode (${CMAKE_BUILD_TYPE})")
  add_compile_definitions(QT_NO_DEBUG_OUTPUT)
endif()

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
    link_directories( ${IMPORT_LIB}
        /home/dmarcini/raspi/sysroot/usr/lib/arm-linux-gnueabihf
        /home/dmarcini/raspi/sysroot/lib/arm-linux-gnueabihf
        )
    #find_library(AVAHICLIENT avahi-client PATHS /home/dmarcini/raspi/sysroot/usr/lib/arm-linux-gnueabihf  DOC "AVAHI Funktionalitaet" ONLY_CMAKE_FIND_ROOT_PATH )
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
else()
    set( EXTERNAL_LIBS ${EXTRA_ZC_LIBS})
    link_directories(${IMPORT_LIB})
endif()
set(IMPORT_LIBS ${lib_name} )
set(QT_CORELIBS Qt5::Core Qt5::Network Qt5::WebSockets Qt5::Xml )

###############################################################################
# die Quellen des Programms definieren                                        #
###############################################################################
set( HEADERS
  main.hpp
  qtzeroconf/qzeroconf.h 
  qtzeroconf/avahi-qt/qt-watch.h
  qtzeroconf/avahi-qt/qt-watch_p.h
  qtzeroconf/qzeroconfservice.h
  qtzeroconf/qzeroconfglobal.h
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

set( SOURCES
  main.cpp
  qtzeroconf/avahiclient.cpp
  qtzeroconf/avahi-qt/qt-watch.cpp
  qtzeroconf/qzeroconfservice.cpp
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




