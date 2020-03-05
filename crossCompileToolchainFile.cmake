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


#SET(Qt5_DIR 
#  ${MOD_ROOT}/Qt5
#  ${MOD_ROOT}/Qt53DAnimation
#  ${MOD_ROOT}/Qt53DCore
#  ${MOD_ROOT}/Qt53DExtras
#  ${MOD_ROOT}/Qt53DInput
#  ${MOD_ROOT}/Qt53DLogic
#  ${MOD_ROOT}/Qt53DQuick
#  ${MOD_ROOT}/Qt53DQuickAnimation
#  ${MOD_ROOT}/Qt53DQuickExtras
#  ${MOD_ROOT}/Qt53DQuickInput
#  ${MOD_ROOT}/Qt53DQuickRender
#  ${MOD_ROOT}/Qt53DQuickScene2D
#  ${MOD_ROOT}/Qt53DRender
#  ${MOD_ROOT}/Qt5AccessibilitySupport
#  ${MOD_ROOT}/Qt5AttributionsScannerTools
#  ${MOD_ROOT}/Qt5Bluetooth
#  ${MOD_ROOT}/Qt5Bodymovin
#  ${MOD_ROOT}/Qt5Bootstrap
#  ${MOD_ROOT}/Qt5BootstrapDBus
#  ${MOD_ROOT}/Qt5Charts
#  ${MOD_ROOT}/Qt5Concurrent
#  ${MOD_ROOT}/Qt5Contacts
#  ${MOD_ROOT}/Qt5Core
#  ${MOD_ROOT}/Qt5DataVisualization
#  ${MOD_ROOT}/Qt5DBus
#  ${MOD_ROOT}/Qt5DeviceDiscoverySupport
#  ${MOD_ROOT}/Qt5DocTools
#  ${MOD_ROOT}/Qt5EdidSupport
#  ${MOD_ROOT}/Qt5EglFSDeviceIntegration
#  ${MOD_ROOT}/Qt5EglSupport
#  ${MOD_ROOT}/Qt5EventDispatcherSupport
#  ${MOD_ROOT}/Qt5FbSupport
#  ${MOD_ROOT}/Qt5Feedback
#  ${MOD_ROOT}/Qt5FontDatabaseSupport
#  ${MOD_ROOT}/Qt5Gamepad
#  ${MOD_ROOT}/Qt5Gui
#  ${MOD_ROOT}/Qt5Help
#  ${MOD_ROOT}/Qt5InputSupport
#  ${MOD_ROOT}/Qt5LinguistTools
#  ${MOD_ROOT}/Qt5Location
#  ${MOD_ROOT}/Qt5Multimedia
#  ${MOD_ROOT}/Qt5MultimediaQuick
#  ${MOD_ROOT}/Qt5MultimediaWidgets
#  ${MOD_ROOT}/Qt5Network
#  ${MOD_ROOT}/Qt5NetworkAuth
#  ${MOD_ROOT}/Qt5Nfc
#  ${MOD_ROOT}/Qt5OpenGL
#  ${MOD_ROOT}/Qt5OpenGLExtensions
#  ${MOD_ROOT}/Qt5Organizer
#  ${MOD_ROOT}/Qt5PacketProtocol
#  ${MOD_ROOT}/Qt5PlatformCompositorSupport
#  ${MOD_ROOT}/Qt5Positioning
#  ${MOD_ROOT}/Qt5PositioningQuick
#  ${MOD_ROOT}/Qt5PrintSupport
#  ${MOD_ROOT}/Qt5PublishSubscribe
#  ${MOD_ROOT}/Qt5Purchasing
#  ${MOD_ROOT}/Qt5Qml
#  ${MOD_ROOT}/Qt5QmlDebug
#  ${MOD_ROOT}/Qt5QmlDevTools
#  ${MOD_ROOT}/Qt5QmlImportScanner
#  ${MOD_ROOT}/Qt5QmlModels
#  ${MOD_ROOT}/Qt5QmlWorkerScript
#  ${MOD_ROOT}/Qt5Quick
#  ${MOD_ROOT}/Qt5Quick3D
#  ${MOD_ROOT}/Qt5Quick3DAssetImport
#  ${MOD_ROOT}/Qt5Quick3DRender
#  ${MOD_ROOT}/Qt5Quick3DRuntimeRender
#  ${MOD_ROOT}/Qt5Quick3DUtils
#  ${MOD_ROOT}/Qt5QuickCompiler
#  ${MOD_ROOT}/Qt5QuickControls2
#  ${MOD_ROOT}/Qt5QuickParticles
#  ${MOD_ROOT}/Qt5QuickShapes
#  ${MOD_ROOT}/Qt5QuickTemplates2
#  ${MOD_ROOT}/Qt5QuickTest
#  ${MOD_ROOT}/Qt5QuickWidgets
#  ${MOD_ROOT}/Qt5RemoteObjects
#  ${MOD_ROOT}/Qt5RepParser
#  ${MOD_ROOT}/Qt5Scxml
#  ${MOD_ROOT}/Qt5Sensors
#  ${MOD_ROOT}/Qt5SerialBus
#  ${MOD_ROOT}/Qt5SerialPort
#  ${MOD_ROOT}/Qt5ServiceFramework
#  ${MOD_ROOT}/Qt5ServiceSupport
#  ${MOD_ROOT}/Qt5Sql
#  ${MOD_ROOT}/Qt5Svg
#  ${MOD_ROOT}/Qt5SystemInfo
#  ${MOD_ROOT}/Qt5Test
#  ${MOD_ROOT}/Qt5TextToSpeech
#  ${MOD_ROOT}/Qt5ThemeSupport
#  ${MOD_ROOT}/Qt5UiPlugin
#  ${MOD_ROOT}/Qt5UiTools
#  ${MOD_ROOT}/Qt5Versit
#  ${MOD_ROOT}/Qt5VersitOrganizer
#  ${MOD_ROOT}/Qt5VirtualKeyboard
#  ${MOD_ROOT}/Qt5WebChannel
#  ${MOD_ROOT}/Qt5WebSockets
#  ${MOD_ROOT}/Qt5WebView
#  ${MOD_ROOT}/Qt5Widgets
#  ${MOD_ROOT}/Qt5Xml
#  ${MOD_ROOT}/Qt5XmlPatterns
#  ${MOD_ROOT}/Qt5Zlib
#)






