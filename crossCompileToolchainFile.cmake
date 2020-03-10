#
# allgemeine cmake Einstellungen
#
cmake_minimum_required(VERSION 3.5)
#
# cross Einstellungen
#
SET(CMAKE_SYSTEM_NAME Linux)
set(CROSS arm-linux-gnueabihf-)
set(POSTFIX raspi)
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
SET(CMAKE_C_COMPILER   /home/dmarcini/raspi/x-tools/arm-linux-gnueabihf/bin/arm-linux-gnueabihf-gcc)
SET(CMAKE_CXX_COMPILER /home/dmarcini/raspi/x-tools/arm-linux-gnueabihf/bin/arm-linux-gnueabihf-g++)
set(AS_EXECUTABLE /home/dmarcini/raspi/x-tools/arm-linux-gnueabihf/bin/arm-linux-gnueabihf-as)

include_directories(/home/dmarcini/raspi/sysroot/usr/include)
SET(Qt5_DIR /home/dmarcini/raspi/qt-5.14.1-ext/lib/cmake/Qt5)
SET(QT_QMAKE_EXECUTABLE /home/dmarcini/raspi/qt-5.14.1-loc/bin/qmake)
set(QTDIR /home/dmarcini/raspi/qt-5-14-1-ext)

#SET(CMAKE_SYSROOT /home/dmarcini/raspi/sysroot )
#SET(CMAKE_PREFIX_PATH "/home/dmarcini/raspi/qt-5.14.1-ext;/home/dmarcini/raspi/sysroot" )
SET(CMAKE_PREFIX_PATH "/home/dmarcini/raspi/sysroot" )

SET(EXTRA_LIB_DIR "/home/dmarcini/raspi/sysroot/usr/local/qt-5.14.1-ext/lib;/home/dmarcini/raspi/sysroot/lib" )

#SET(CMAKE_FIND_ROOT_PATH 
#  /home/dmarcini/raspi/qt-5.14.1-ext
#  /home/dmarcini/raspi/qt-5.14.1-ext/lib
#  /home/dmarcini/raspi/sysroot
#  home/dmarcini/raspi/sysroot/lib 
#  home/dmarcini/raspi/sysroot/lib/arm-linux-gnueabihf 
#  home/dmarcini/raspi/sysroot/usr/lib
#  home/dmarcini/raspi/sysroot/usr/lib/arm-linux-gnueabihf 
#)


#
# Modul Suchpfade
#
SET( MOD_ROOT /home/dmarcini/raspi/qt-5.14.1-ext/lib/cmake ) 
SET(Qt5Core_DIR ${MOD_ROOT}/Qt5Core)
SET(Qt5WebSockets_DIR ${MOD_ROOT}/Qt5WebSockets)
SET(Qt5Network_DIR ${MOD_ROOT}/Qt5Network)
SET(Qt5Xml_DIR ${MOD_ROOT}/Qt5Xml)

#
# RPATH
#
#set(CMAKE_SKIP_BUILD_RPATH FALSE)

