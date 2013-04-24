QT += network dbus
CONFIG += C++11


HEADERS += \
    lib/github.h \
    keyring.h \
    lib/gitlab.h \
    lib/http.h \
    lib/utils.h \
    lib/gitlabTest.h

SOURCES += \
    main.cpp

OTHER_FILES += \
    doc/spec.txt \
    doc/interface.txt
