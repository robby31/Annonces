TEMPLATE = app

QT += qml quick widgets xml sql

CONFIG += c++11

SOURCES += main.cpp \
    homepagecontroller.cpp \
    annoncesapplication.cpp \
    applicationworker.cpp \
    annonceslist.cpp \
    leboncoinlist.cpp \
    annonce.cpp \
    leboncoin.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH = C:/Users/NJUT/Documents/workspaceQT/robby31-qmlmodulesplugins

# Default rules for deployment.
include(deployment.pri)

include (../QmlApplication/QmlApplication.prf)

HEADERS += \
    homepagecontroller.h \
    annoncesapplication.h \
    applicationworker.h \
    annonceslist.h \
    leboncoinlist.h \
    annonce.h \
    leboncoin.h
