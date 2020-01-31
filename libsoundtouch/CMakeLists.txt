cmake_minimum_required(VERSION 3.5)

project(soundtouch_qt LANGUAGES CXX)

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

###############################################################################
# Projekt definitionen                                                        #
###############################################################################
set(KIT QT5141)
set(MAJOR 1) # major version number
set(MINOR 0) # minor version number
set(PATCH 5) # patch number
set(BUILD 0) # win32: build number
if (WIN32)
  # major.minor.patch.build
  set(VERSION $${MAJOR}.$${MINOR}.$${PATCH}.$${BUILD})
else()
  # major.minor.patch
  set(VERSION $${MAJOR}.$${MINOR}.$${PATCH})
endif()

if (CMAKE_BUILD_TYPE  STREQUAL "Debug")
  set(lib_name soundtouch_debug_qt)
  message("create in debug mode (${CMAKE_BUILD_TYPE})")
  add_compile_definitions(DEBUG)
else()
  message("create in release mode (${CMAKE_BUILD_TYPE})")
  set(lib_name soundtouch_qt)
  add_compile_definitions(QT_NO_DEBUG_OUTPUT)
endif()

set(qt_defs SOUNDTOUCH_QT_LIB_EXPORT QT_DEPRECATED_WARNINGS)
set(prj_version VMAJOR=${MAJOR} VMINOR=${MINOR} VPATCH=${PATCH})

add_compile_definitions(${qt_defs} ${prj_version})

# da hin sollen die includedateien exportiert werden
set(EXPORT_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/../include)
set(EXPORT_LIB ${CMAKE_CURRENT_SOURCE_DIR}/../lib)
set(DESTDIR ${EXPORT_LIB}/${KIT})
message("INCLUDE export to ${EXPORT_INCLUDE}")
message("LIBRARY export to directory ${EXPORT_LIB}")
message("DESTINATION for library: ${DESTDIR}")

find_package(Qt5 COMPONENTS Core WebSockets Network Xml REQUIRED)

set(SOURCES
  bsoundtouchdevice.cpp
  xmlparser/xmlresultparser.cpp
  xmlparser/iresponseobject.cpp
  websocket/bwebsocket.cpp
  xmlparser/xmlupdateparser.cpp
  xmlparser/bsoundtouch_global.cpp
  xmlparser/httpresponse/httppresetsobject.cpp
  xmlparser/httpresponse/httpzoneobject.cpp
  xmlparser/httpresponse/httpvolumeobject.cpp
  xmlparser/httpresponse/httpsourcesobject.cpp
  xmlparser/httpresponse/httpresultokobject.cpp
  xmlparser/httpresponse/httpresulterrorobject.cpp
  xmlparser/httpresponse/httpnowplayingobject.cpp
  xmlparser/httpresponse/httpgroupobject.cpp
  xmlparser/httpresponse/httpdeviceinfoobject.cpp
  xmlparser/httpresponse/httpbassobject.cpp
  xmlparser/httpresponse/httpbasscapabilitiesobject.cpp
  xmlparser/wscallback/wssoundtouchsdkinfo.cpp
  xmlparser/wscallback/wsuseractivityupdated.cpp
  xmlparser/wscallback/wszoneupdated.cpp
  xmlparser/wscallback/wsvolumeupdated.cpp
  xmlparser/wscallback/wsuserinactivityupdated.cpp
  xmlparser/wscallback/wssourceupdated.cpp
  xmlparser/wscallback/wsrecentsupdated.cpp
  xmlparser/wscallback/wspresetupdateobject.cpp
  xmlparser/wscallback/wsnowselectionupdated.cpp
  xmlparser/wscallback/wsnowplayingupdate.cpp
  xmlparser/wscallback/wsnameupdated.cpp
  xmlparser/wscallback/wslanguageupdated.cpp
  xmlparser/wscallback/wsinfoupdated.cpp
  xmlparser/wscallback/wsgroupupdated.cpp
  xmlparser/wscallback/wserrorupdated.cpp
  xmlparser/wscallback/wsconnectionstateupdated.cpp
  xmlparser/wscallback/wsbrowseupdated.cpp
  xmlparser/wscallback/wsbassupdated.cpp
  xmlparser/wscallback/wsaudiospcontrols.cpp
  xmlparser/wscallback/wsaudioproducttonecontrols.cpp
  xmlparser/wscallback/wsaudioproductlevelcontrols.cpp
)

set( HEADERS
  xmlparser/xmlresultparser.hpp
  xmlparser/iresponseobject.hpp
  xmlparser/bsoundtouch_global.hpp
  bsoundtouchdevice.hpp
  websocket/bwebsocket.hpp
  xmlparser/xmlupdateparser.hpp
  xmlparser/httpresponse/httppresetsobject.hpp
  xmlparser/wscallback/wszoneupdated.hpp
  xmlparser/wscallback/wsvolumeupdated.hpp
  xmlparser/wscallback/wsuserinactivityupdated.hpp
  xmlparser/wscallback/wsuseractivityupdated.hpp
  xmlparser/wscallback/wssourceupdated.hpp
  xmlparser/wscallback/wssoundtouchsdkinfo.hpp
  xmlparser/wscallback/wsrecentsupdated.hpp
  xmlparser/wscallback/wspresetupdateobject.hpp
  xmlparser/wscallback/wsnowselectionupdated.hpp
  xmlparser/wscallback/wsnowplayingupdate.hpp
  xmlparser/wscallback/wsnameupdated.hpp
  xmlparser/wscallback/wslanguageupdated.hpp
  xmlparser/wscallback/wsinfoupdated.hpp
  xmlparser/wscallback/wsgroupupdated.hpp
  xmlparser/wscallback/wserrorupdated.hpp
  xmlparser/wscallback/wsconnectionstateupdated.hpp
  xmlparser/wscallback/wsbrowseupdated.hpp
  xmlparser/wscallback/wsbassupdated.hpp
  xmlparser/wscallback/wsaudiospcontrols.hpp
  xmlparser/wscallback/wsaudioproducttonecontrols.hpp
  xmlparser/wscallback/wsaudioproductlevelcontrols.hpp
  xmlparser/httpresponse/httpzoneobject.hpp
  xmlparser/httpresponse/httpvolumeobject.hpp
  xmlparser/httpresponse/httpsourcesobject.hpp
  xmlparser/httpresponse/httpresultokobject.hpp
  xmlparser/httpresponse/httpresulterrorobject.hpp
  xmlparser/httpresponse/httpnowplayingobject.hpp
  xmlparser/httpresponse/httpgroupobject.hpp
  xmlparser/httpresponse/httpdeviceinfoobject.hpp
  xmlparser/httpresponse/httpbassobject.hpp
  xmlparser/httpresponse/httpbasscapabilitiesobject.hpp
)

add_library( ${lib_name} SHARED
  ${HEADERS}
  ${SOURCES}
)

set(common_libs  )
set(qt_libs Qt5::Core Qt5::Network Qt5::WebSockets Qt5::Xml)

target_link_libraries(${lib_name} PRIVATE ${qt_libs} ${common_libs})
target_compile_definitions(${lib_name} PRIVATE LIBSOUNDTOUCH_LIBRARY)


###############################################################################
# POST Build                                                                  #
###############################################################################
#add_custom_command(TARGET ${lib_name} POST_BUILD
#add_custom_command(TARGET deploy
add_custom_target(deploy
    DEPENDS ${lib_name}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Running deployment..."
    COMMAND pwd
    COMMAND touch ${EXPORT_INCLUDE}/toDel
    COMMAND rm -rf ${EXPORT_INCLUDE}/*
    COMMAND cp -f --parents ${HEADERS} ${EXPORT_INCLUDE}
    COMMAND touch ${EXPORT_LIB}/${lib_name}.del
    COMMAND rm -f ${EXPORT_LIB}/${lib_name}.*
    COMMAND cp -f ${CMAKE_CURRENT_BINARY_DIR}/${lib_name}.* ${EXPORT_LIB}/
    COMMAND echo "BUILD DIR: ${CMAKE_CURRENT_BINARY_DIR}"
)




