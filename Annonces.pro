TEMPLATE = app

QT += qml quick widgets xml charts

CONFIG += c++11

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

include (../QmlApplication/QmlApplication.prf)

HEADERS += \
    homepagecontroller.h \
    annoncesapplication.h \
    applicationworker.h \
    annonceslist.h \
    leboncoinlist.h \
    annonce.h \
    leboncoin.h \
    pricemodel.h
