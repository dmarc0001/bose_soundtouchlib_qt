cmake_minimum_required(VERSION 3.5)

project(sb_browser LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(qt_defs QT_DEPRECATED_WARNINGS)

link_directories( /opt/qt/5.14.1/gcc_64/lib )
set( Z_EXTLIB QtZeroConf )

add_compile_definitions(${qt_defs})

find_package(Qt5 COMPONENTS Network REQUIRED)
#find_package(Qt5Core)

add_executable(sb_browser
  main.cpp
  soundtouchdiscover.cpp
  soundtouchdiscover.hpp
)
target_link_libraries(sb_browser PRIVATE Qt5::Core Qt5::Network ${Z_EXTLIB} )
