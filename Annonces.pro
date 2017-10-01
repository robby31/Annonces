TEMPLATE = app

QT += qml quick widgets xml charts sql webview

CONFIG += c++11

CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT

SOURCES += main.cpp \
    homepagecontroller.cpp \
    annoncesapplication.cpp \
    applicationworker.cpp \
    annonceslist.cpp \
    leboncoinlist.cpp \
    annonce.cpp \
    leboncoin.cpp \
    pricemodel.cpp

RESOURCES += qml.qrc

# Default rules for deployment.
include (deployment.pri)

INCLUDEPATH += $$(MYLIBRARY)/$$QT_VERSION/include/QmlApplication
LIBS += -L$$(MYLIBRARY)/$$QT_VERSION -l$$qtLibraryTarget(QmlApplication)

HEADERS += \
    homepagecontroller.h \
    annoncesapplication.h \
    applicationworker.h \
    annonceslist.h \
    leboncoinlist.h \
    annonce.h \
    leboncoin.h \
    pricemodel.h
