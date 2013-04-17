QT += network dbus
CONFIG += C++11


HEADERS += \
    github.h \
    keyring.h

SOURCES += \
    github.cpp \
    main.cpp

OTHER_FILES += \
    doc/spec.txt \
    doc/interface.txt
