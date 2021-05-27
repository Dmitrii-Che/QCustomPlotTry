QT += core gui
QT += core
QT += widgets printsupport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CHDV-QCP
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS


CONFIG += c++14
SOURCES += \
        main.cpp \
        chdvplot.cpp \
    qcustomplot.cpp \
    chdvreader.cpp \
    chdvcheckboxwgt.cpp \

HEADERS += \
        chdvplot.h \
    qcustomplot.h \
    chdvreader.h \
    chdvcheckboxwgt.h \
