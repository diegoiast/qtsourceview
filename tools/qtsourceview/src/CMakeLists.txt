# src - library 

PROJECT(qtsourceview)
CMAKE_MINIMUM_REQUIRED(VERSION 2.4.0)

# input files
SET( qtsourceview_SRCS		qsvcolordef.cpp qsvcolordeffactory.cpp qsvlangdef.cpp qsvlangdeffactory.cpp qsvsyntaxhighlighter.cpp )
# SET( qtsourceview_MOC_HDRS	qorderedmap.h qsvcolordef.h qsvcolordeffactory.h qsvlangdef.h qsvlangdeffactory.h qsvsyntaxhighlighter.h debug_info.h )
SET( qtsourceview_RCS		qtsourceview.qrc )

# verbose - on
# SET( CMAKE_VERBOSE_MAKEFILE  ON )

# definitions for the project
ADD_DEFINITIONS( -Wall )
SET( QT_USE_QTXML TRUE )
SET( QT_USE_QTGUI TRUE )
FIND_PACKAGE( Qt4 REQUIRED )
LINK_DIRECTORIES( ../../ )
INCLUDE(${QT_USE_FILE})

# qt4 definitinos
QT4_WRAP_CPP		( qtsourceview_MOC_SRCS	${qtsourceview_MOC_HDRS}	)
QT4_AUTOMOC		( ${qtsourceview_SRCS}					)
QT4_ADD_RESOURCES	( qtsourceview_RC_SRCS ${SAMPLE_RCS}				)

# build the file
INCLUDE_DIRECTORIES	( ${CMAKE_BINARY_DIR} )
ADD_LIBRARY		( qtsourceview	${qtsourceview_SRCS} ${qtsourceview_MOC_SRCS} ${qtsourceview_RC} )
