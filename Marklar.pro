greaterThan(QT_MAJOR_VERSION, 4) {
    QT += widgets
    DEFINES += AM_USING_QT5
    }
else {
    DEFINES += AM_USING_QT4
    }

TEMPLATE		= app
CONFIG			+= qt warn_off release
CONFIG			-= app_bundle

FORMS			=

HEADERS			= Hive.h \
			  Marklar.h

SOURCES			= Main.cpp \
			  Hive.cpp \
			  Marklar.cpp

RESOURCES		= main.qrc
win32:RC_FILE		+= metainfo.rc
macx:ICON		+= icon.icns

TARGET			= Marklar
OBJECTS_DIR		= .temp
MOC_DIR			= .temp
UI_DIR			= .temp
UI_HEADERS_DIR		= .temp
UI_SOURCES_DIR		= .temp

