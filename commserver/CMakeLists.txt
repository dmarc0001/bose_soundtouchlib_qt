cmake_minimum_required(VERSION 3.5)

project(commserver LANGUAGES CXX)

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
set(MINOR 1) # minor version number
set(PATCH 0) # patch number
set(BUILD 0) # win32: build number
if (WIN32)
  # major.minor.patch.build
  set(VERSION $${MAJOR}.$${MINOR}.$${PATCH}.$${BUILD})
else()
  # major.minor.patch
  set(VERSION $${MAJOR}.$${MINOR}.$${PATCH})
endif()
set(IMPORT_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/../libsoundtouch")
set(IMPORT_LIB "${CMAKE_CURRENT_SOURCE_DIR}/../lib")

###############################################################################
# DEBUG oder kein DEBUG                                                       #
###############################################################################
if (CMAKE_BUILD_TYPE  STREQUAL "Debug")
  set(prog_name commserver_debug)
  set(lib_name soundtouch_debug)
  message(" ==> create ${prog_name} in debug mode (${CMAKE_BUILD_TYPE})")
  add_compile_definitions(DEBUG)
else()
  set(prog_name commserver)
  set(lib_name soundtouch)
  message(" ==> create  ${prog_name} in release mode (${CMAKE_BUILD_TYPE})")
  add_compile_definitions(QT_NO_DEBUG_OUTPUT)
endif()

###############################################################################
# Defines für den Compiler                                                    #
###############################################################################
set(qt_defs SOUNDTOUCH_QT_LIB_IMPORT QT_DEPRECATED_WARNINGS)
set(prj_version VMAJOR=${MAJOR} VMINOR=${MINOR} VPATCH=${PATCH})
add_compile_definitions(${qt_defs} ${prj_version})

###############################################################################
# cmake definitionen für module finden                                        #
###############################################################################
find_package(Qt5 COMPONENTS Core WebSockets Network REQUIRED)
#
# wenn die lib da ist oder nicht
#find_path(SOUNDTOUCH_LIB_PATH ${IMPORT_LIB})
#find_library(SOUNDTOUCH_LIB ${lib_name} REQUIRED )

###############################################################################
# Orte der Bibliothek bekannt geben                                           #
###############################################################################
include_directories(${IMPORT_INCLUDE})
link_directories(${IMPORT_LIB})

###############################################################################
# die Quellen des Programms definieren                                        #
###############################################################################
set(HEADERS
  main.hpp
  config/programconfig.hpp
  config/DaemonConfig.hpp
  commserver/BoseCommServer.hpp
)

set(SOURCES
  main.cpp
  config/programconfig.cpp
  config/DaemonConfig.cpp
  commserver/BoseCommServer.cpp
)


add_executable(${prog_name}
  ${HEADERS}
  ${SOURCES}
)

#
set(IMPORT_LIBS ${lib_name} )
set(QT_CORELIBS Qt5::Core Qt5::Network Qt5::WebSockets)
# und zusammenfassen zum Programm
target_link_libraries(${prog_name} PRIVATE ${QT_CORELIBS} ${IMPORT_LIBS})





