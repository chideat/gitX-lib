QT += network dbus
CONFIG += C++11


HEADERS += \
    lib/github.h \
    keyring.h \
    lib/gitlab.h \
    lib/http.h \
    lib/utils.h \
    ui/window.h \
    lib/gitlab/test.h \
    lib/gitx.h \
    lib/notification.h \
    lib/gitlab/log.h

SOURCES += \
    main.cpp \
    ui/window.cpp \
    lib/gitlab/test.cpp \
    lib/notification.cpp

OTHER_FILES += \
    doc/spec.txt \
    doc/interface.txt
