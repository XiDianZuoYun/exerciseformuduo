#-------------------------------------------------
#
# Project created by QtCreator 2018-04-20T15:29:15
#
#-------------------------------------------------

QT       -= gui
CONFIG +=c++14
TARGET = test
TEMPLATE = lib

DEFINES += TEST_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    POLLER.cpp \
    eventloop.cpp \
    Channel.cpp \
    THREAD.cpp \
    CONDITION.cpp \
    MUTEXLOCK.cpp \
    threadpool.cpp \
    oopsocket.cpp \
    main.cpp \
    buffer.cpp

HEADERS += \
    POLLER.hpp \
    Channel.hpp \
    eventloop.hpp \
    threadpool.hpp \
    THREAD.h \
    MUTEXLOCK.hpp \
    oopsocket.h \
    CONDITION.hpp \
    buffer.hpp

unix {
    target.path = /usr/lib
    INSTALLS += target
}
