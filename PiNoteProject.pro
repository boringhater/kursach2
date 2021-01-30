QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

VERSION = 0
QMAKE_TARGET_COMPANY = CppCppIVProduction
QMAKE_TARGET_PRODUCT = PiNote Kursach
QMAKE_TARGET_DESCRIPTION = One pretty kursach
QMAKE_TARGET_COPYRIGHT = CppCppIVProduction



CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    PiNoteLib.cpp \
    createfilewindow.cpp \
    editnotewindow.cpp \
    filtrationwindow.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    PiNoteLib.h \
    createfilewindow.h \
    editnotewindow.h \
    filtrationwindow.h \
    mainwindow.h

FORMS += \
    createfilewindow.ui \
    editnotewindow.ui \
    filtrationwindow.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
