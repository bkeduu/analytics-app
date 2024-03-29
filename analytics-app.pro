QT       += core gui network charts statemachine

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17 static

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    client.cpp \
    etc/stackedbarwithlineswidget.cpp \
    etc/utils.cpp \
    main.cpp \
    mainwindow.cpp \
    tabs/consumerstab.cpp \
    tabs/forecasttab.cpp \
    tabs/generationtab.cpp \
    tabs/itab.cpp \
    tabs/statustab.cpp \
    networking/connection.cpp \
    etc/customwidgets.cpp \
    etc/contentwidget.cpp \
    etc/bcswitch.cpp \
    etc/callout.cpp \
    etc/calloutwidget.cpp \
    etc/calloutview.cpp

HEADERS += \
    client.h \
    etc/stackedbarwithlineswidget.h \
    etc/utils.h \
    etc/bcswitch.h \
    etc/contentwidget.h \
    etc/callout.h \
    etc/calloutwidget.h \
    etc/calloutview.h \
    mainwindow.h \
    etc/customwidgets.h \
    tabs/consumerstab.h \
    tabs/forecasttab.h \
    tabs/generationtab.h \
    tabs/itab.h \
    tabs/statustab.h \
    networking/connection.h


FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    analytics-app_ru_RU.ts

CONFIG += lrelease
CONFIG += embed_translations

INCLUDEPATH += tabs networking etc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
	analytics-app.qrc
