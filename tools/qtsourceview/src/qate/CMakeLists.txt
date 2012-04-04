PROJECT(qtsourceview)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.0)
set(QATE_VERSION "0.0.3")
set(QATE_SOVERSION "0")

# $Id$
# definitions for the project
# SET( CMAKE_VERBOSE_MAKEFILE  ON )
SET( QT_USE_QTXML TRUE )
SET( QT_USE_QTGUI TRUE )
SET( QT_USE_QTNETWORK TRUE )
FIND_PACKAGE( Qt4 REQUIRED )
INCLUDE(${QT_USE_FILE})

add_definitions(-DCORE_EXPORT=)
# input files
#	highlightersettings.cpp
SET( QATE_SRCS
	context.cpp
	defaultcolors.cpp
	definitiondownloader.cpp
	dynamicrule.cpp
	highlightdefinition.cpp
	highlightdefinitionhandler.cpp
	highlightdefinitionmanager.cpp
	highlightdefinitionmetadata.cpp
	highlighter.cpp
	includerulesinstruction.cpp
	itemdata.cpp
	keywordlist.cpp
	mimedatabase.cpp
	progressdata.cpp
	rule.cpp
	specificrules.cpp
)
SET( QATE_MOC_HDRS
	definitiondownloader.h
	highlightdefinitionmanager.h
	highlighter.h
)
#SET( QATE_RCS		qtsourceview.qrc )

# qt4 definitinos
QT4_AUTOMOC       ( ${QATE_SRCS} )
QT4_WRAP_CPP      ( QATE_MOC_SRCS ${QATE_MOC_HDRS} )
#QT4_ADD_RESOURCES ( QATE_RC_SRCS ${QATE_RCS} )

# build the file
INCLUDE_DIRECTORIES	( ../ .)
ADD_LIBRARY( QATE_STATIC STATIC ${QATE_SRCS} ${QATE_MOC_SRCS}  )
ADD_LIBRARY( QATE_SHARED SHARED ${QATE_SRCS} ${QATE_MOC_SRCS} ${QATE_RC_SRCS} )
SET_TARGET_PROPERTIES(
  QATE_SHARED
    PROPERTIES
      VERSION     ${QATE_VERSION}
      SOVERSION   ${QATE_SOVERSION}
)

if(MSVC)
	set_target_properties(QATE_STATIC PROPERTIES OUTPUT_NAME qsvsh_static)
 	set_target_properties(QATE_SHARED PROPERTIES OUTPUT_NAME qsvsh)
else()
	set_target_properties(QATE_STATIC PROPERTIES OUTPUT_NAME qsvsh)
 	set_target_properties(QATE_SHARED PROPERTIES OUTPUT_NAME qsvsh)
endif()


ADD_EXECUTABLE(DEMO_QATE ../../demos/demo-qate/demo-qate.cpp)
TARGET_LINK_LIBRARIES(DEMO_QATE QATE_STATIC ${QT_LIBRARIES} )
SET_TARGET_PROPERTIES(DEMO_QATE PROPERTIES OUTPUT_NAME qate-demo)

QT4_WRAP_CPP(DEMO_QATE2_MOC ../../demos/demo-qate2/demo-qate2.h )
ADD_EXECUTABLE(DEMO_QATE2 ../../demos/demo-qate2/demo-qate2.cpp ${DEMO_QATE2_MOC})
TARGET_LINK_LIBRARIES(DEMO_QATE2 QATE_STATIC ${QT_LIBRARIES} )
SET_TARGET_PROPERTIES(DEMO_QATE2 PROPERTIES OUTPUT_NAME qate-demo2)
