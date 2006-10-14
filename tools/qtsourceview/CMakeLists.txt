# main project

PROJECT(qtsourceview) 
CMAKE_MINIMUM_REQUIRED(VERSION 2.4.0)

FIND_PACKAGE( Qt4 REQUIRED )
INCLUDE(${QT_USE_FILE})

# tell cmake to process CMakeLists.txt in that subdirectory
ADD_SUBDIRECTORY(src)
# ADD_SUBDIRECTORY(demos/demo1 ${CMAKE_BINARY_DIR} )
ADD_SUBDIRECTORY(demos/demo2 ${CMAKE_BINARY_DIR} )
# ADD_SUBDIRECTORY(demos/demo3 ${CMAKE_BINARY_DIR} )
