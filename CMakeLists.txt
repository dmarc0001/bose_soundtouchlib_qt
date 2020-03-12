cmake_minimum_required(VERSION 3.5)

###############################################################################
# übergeordnetes Projekt für die Library und den Test derselben               #
###############################################################################
project(bose_lib)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)

if(UNIX)
  add_compile_definitions(UNIX)
endif(UNIX)
if(WIN32)
    add_compile_definitions(WIN32)
endif(WIN32)

if (CMAKE_CROSSCOMPILING)
    message("############### PROJECT SOUNDTOUCH CROSS COMPILER ###################")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/rpilib)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/rpilib)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/rpilib)
else()
    message("############### PROJECT SOUNDTOUCH NATIVE COMPILER ###################")
endif()


add_subdirectory(libsoundtouch)
#add_subdirectory(test_soundtouch)
add_subdirectory(alert_daemon)
add_subdirectory(testclient)

#add_dependencies(commserver libsoundtouch)
