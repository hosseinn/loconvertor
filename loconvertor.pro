TEMPLATE = app
TARGET = loconvertor
CONFIG += console

QMAKE_CXXFLAGS += -std=c++17

LOROOT = /opt/libreoffice7.6

DEFINES += LO_ROOT='\\"$${LOROOT}\\"'

INCLUDEPATH += $${LOROOT}/sdk/include \
        com/sun/star

SOURCES += \
    main.cpp \

LIBS += -L$${LOROOT}/sdk/lib \
        -L$${LOROOT}/program \
        -luno_sal \
        -luno_cppu \
        -luno_cppuhelpergcc3 \
        -luno_salhelpergcc3 \
        -lunoidllo \
        -lxmlreaderlo \
        -lreglo

DEFINES += LINUX

HEADERS += \

DISTFILES += \
    README.md \
    test.odt

!build_pass:prebuild.commands = $${LOROOT}/sdk/bin/cppumaker -Gc -O. $${LOROOT}/program/types.rdb $${LOROOT}/program/types/offapi.rdb
!build_pass:FORCE.depends += prebuild
QMAKE_EXTRA_TARGETS += FORCE prebuild
