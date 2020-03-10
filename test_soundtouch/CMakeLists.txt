cmake_minimum_required(VERSION 3.5)

project(soundtouch_test LANGUAGES CXX)

###############################################################################
#                                                                             #
# compiler Definitionen                                                       #
#                                                                             #
###############################################################################
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (CMAKE_CROSSCOMPILING)
    message("============= CROSSCOMPILER ========================")
    set(QTDIR /home/dmarcini/raspi/qt-5-14-1-loc)
endif()

###############################################################################
# Projekt definitionen                                                        #
###############################################################################
set(KIT QT5141)
set(MAJOR 1) # major version number
set(MINOR 0) # minor version number
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
  set(prog_name soundtouch_debug_test)
  set(lib_name soundtouch_debug)
  message(" ==> create ${prog_name} in debug mode (${CMAKE_BUILD_TYPE})")
  add_compile_definitions(DEBUG)
else()
  set(prog_name soundtouch_test)
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
find_package(Qt5 COMPONENTS Widgets Gui Network WebSockets Xml REQUIRED)

###############################################################################
# Orte der Bibliothek bekannt geben                                           #
###############################################################################
#add_library(libsoundtouch)
include_directories(${IMPORT_INCLUDE})
link_directories(${IMPORT_LIB})

###############################################################################
# die Quellen des Programms definieren                                        #
###############################################################################
set(SOURCES
  main.cpp
  librarytestwindow.cpp
  config/programconfig.cpp
)

set(HEADERS
  librarytestwindow.hpp
  config/programconfig.hpp
)

set(FORMS
   librarytestwindow.ui
)
#MainWindow.ui

#
# zusammenfassen für compilierung
#
add_executable(${prog_name}
  ${FORMS}
  ${HEADERS}
  ${SOURCES}
)

###############################################################################
# Bibliotheken definieren                                                     #
###############################################################################
set(common_libs ${lib_name} )
set(qt_libs Qt5::Widgets Qt5::Core Qt5::Network Qt5::WebSockets Qt5::Xml)
# und zusammenfassen zum Programm
target_link_libraries(${prog_name} PRIVATE ${qt_libs} ${common_libs})

install(TARGETS ${prog_name}
  RUNTIME
    DESTINATION ${IMPORT_LIB}
  LIBRARY
    DESTINATION ${IMPORT_LIB}
 )

#install(TARGETS ${prog_name}
#  ARCHIVE
#    DESTINATION ${EXPORT_LIB}
#  LIBRARY
#    DESTINATION ${EXPORT_LIB}
#  RUNTIME
#    DESTINATION ${EXPORT_LIB}
#  FRAMEWORK
#    DESTINATION ${EXPORT_LIB}
#)


###############################################################################
# die Bibliothek in den ordner kopieren, um das programm starten zu können    #
###############################################################################
add_custom_command(TARGET ${prog_name}
  POST_BUILD
  COMMAND cp -f ${IMPORT_LIB}/${lib_name}* ${CMAKE_CURRENT_BINARY_DIR}/
)


















