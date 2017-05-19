#-------------------------------------------------
#
# Project created by QtCreator 2016-08-28T19:46:12
#
#-------------------------------------------------

QT      += gui core
CONFIG  += debug_and_release build_all
TEMPLATE = lib
DESTDIR = ./bin

CONFIG(debug, debug|release) {
    message(for debug)
    OBJECTS_DIR = ./build/debug
    MOC_DIR = ./build/debug
    TARGET = hmlibD
}

CONFIG(release, debug|release) {
    message(for release)
    OBJECTS_DIR = ./build/release
    MOC_DIR = ./build/release
    TARGET = hmlib
}

DEFINES += HMLIB_LIBRARY BOOST_ALL_DYN_LINK

SOURCES += hmlib.cpp \
    src/msg/msg.cpp \
    src/msg/msgBus.cpp \
    src/msg/msgHander.cpp \
    src/service/QueueService.cpp \
    src/service/Service.cpp \
    src/service/ServiceImpl.cpp \
    src/service/ServiceTimer.cpp \
    src/service/ServiceTimerImpl.cpp \
    src/service/Worker.cpp \
    src/service/WorkerImpl.cpp \
    src/utils/utils.cpp \
    src/msg/msgSignal.cpp

HEADERS += hmlib.h\
        hmlib_global.h \
    include/msg/msg.h \
    include/msg/msgBus.h \
    include/msg/msgHander.h \
    src/service/serviceImpl.h \
    src/service/serviceTimerImpl.h \
    src/service/workerImpl.h \
    include/service/queueService.h \
    include/service/service.h \
    include/service/serviceTimer.h \
    include/service/worker.h \
    include/msg/gui/msgHanderView.h \
    include/msg/gui/msgHanderView.hpp \
    include/utils/utils.h \
    include/msg/msgSignal.h


unix {
    target.path = /usr/lib
    INSTALLS += target
}
LIBS += -L$$(boost_lib)/lib32-msvc-12.0

INCLUDEPATH += $$(boost_lib) \
                include/msg   \
                include/utils \
                include/service

DISTFILES += \
    common.pri
