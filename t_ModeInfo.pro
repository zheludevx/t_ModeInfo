TEMPLATE =	app

win32 :	DEFINES -= UNICODE

CONFIG -= qt
CONFIG += \
          warn_on \ 
          thread 

unix {
    LIBS +=-ldl -lboost_filesystem -lboost_system
    TARGET=$${TARGET}.bin
}

win32 {
    LIBS += \
	-lws2_32 \
	-luser32
}

NITA_LIBS += \
#	-lBaseSet \
#	-lreg_12 \
#	-lOSIndependent

CONFIG(debug, debug|release) {
	OBJECTS_DIR = tmp/debug
	DEFINES += _DEBUG
	DEFINES -= NDEBUG


}
else {
	OBJECTS_DIR = tmp/release
	DEFINES -= _DEBUG
	DEFINES += NDEBUG
}

SOURCES += \
           main.cpp
