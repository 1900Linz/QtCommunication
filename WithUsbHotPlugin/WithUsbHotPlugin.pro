QT +=  core gui widgets

CONFIG += c++17


DEFINES += QT_DEPRECATED_WARNINGS

HEADERS += \
    DevPluginUSB.h

SOURCES += \
        DevPluginUSB.cpp \
        main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


