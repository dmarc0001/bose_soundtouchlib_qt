#
# allgemeine cmake Einstellungen
#
cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#
# cross Einstellungen
#
SET(CMAKE_SYSTEM_NAME Linux)
# prefix für libs
set(PREFIX_QT /home/dmarcini/raspi/qt-5.14.1-ext )
SET(CMAKE_PREFIX_PATH ${PREFIX_QT}/lib/cmake/Qt5  )
SET(CMAKE_FIND_ROOT_PATH  /home/dmarcini/raspi/sysroot )
SET(QT_QMAKE_EXECUTABLE /home/dmarcini/raspi/qt-5.14.1-loc/bin/qmake)
#
# search for programs in the build host directories
#
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
#
# for libraries and headers in the target directories only
#
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
#
# cross compiler CMAKE_FIND_ROOT_PATH_MODE_INCLUDE
#
SET(CMAKE_C_COMPILER   /home/dmarcini/raspi/x-tools/arm-linux-gnueabihf/bin/arm-linux-gnueabihf-gcc)
SET(CMAKE_CXX_COMPILER /home/dmarcini/raspi/x-tools/arm-linux-gnueabihf/bin/arm-linux-gnueabihf-g++)


