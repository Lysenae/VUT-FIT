TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
INCLUDEPATH = /usr/include/libxml2
LIBS += -L/usr/lib -libxml2

SOURCES += \
    arppkt.cpp \
    pds-scanner.cpp \
    ipv4addr.cpp \
    ipv6addr.cpp \
    ipaddr.cpp \
    socket.cpp \
    macaddr.cpp \
    netitf.cpp \
    types.cpp \
    packet.cpp \
    hash.cpp \
    icmpv6pkt.cpp \
    pds-spoof.cpp \
    pds-intercept.cpp \
    hostgroup.cpp

HEADERS += \
    arppkt.h \
    ipv4addr.h \
    ipv6addr.h \
    types.h \
    ipaddr.h \
    socket.h \
    macaddr.h \
    netitf.h \
    packet.h \
    hash.h \
    icmpv6pkt.h \
    hostgroup.h

DISTFILES += \
    Makefile
