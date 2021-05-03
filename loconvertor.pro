TEMPLATE = app
TARGET = loconvertor
CONFIG += console

QMAKE_CXXFLAGS += -std=c++17

# If you use LibreOffice 7.1 SDK, use this
# Due to LibreOffice bug #14186:
#     https://bugs.documentfoundation.org/show_bug.cgi?id=141896
# you have to create a symbolic link in /libmergedlo.so:
#
# sudo ln -s /opt/libreoffice7.1/program/libmergedlo.so /libmergedlo.so
LOROOT = /opt/libreoffice7.1

# If you have built LO use the instdir path here:
# But first you have to build LibreOffice.
#     https://wiki.documentfoundation.org/Development/BuildingOnLinux
#LOROOT = /home/hossein/Projects/libreoffice/core/instdir

DEFINES += LO_ROOT='\\"$${LOROOT}\\"'

INCLUDEPATH += $${LOROOT}/sdk/include \
    ../UnoApiHeaders7.1

SOURCES += \
    main.cpp \

LIBS += -L$${LOROOT}/sdk/lib \
        -L$${LOROOT}/program \
        -luno_sal \
        -luno_cppu \
        -luno_cppuhelpergcc3 \
        -lxmlreaderlo \
        -lunoidllo \
        -luno_salhelpergcc3 \
        -lreglo

DEFINES += LINUX

HEADERS += \

DISTFILES += \
    README.md \
    test.odt

!build_pass:prebuild.commands = $${LOROOT}/sdk/bin/cppumaker -Gc -O. $${LOROOT}/program/types.rdb $${LOROOT}/program/types/offapi.rdb
!build_pass:FORCE.depends += prebuild
QMAKE_EXTRA_TARGETS += FORCE prebuild
