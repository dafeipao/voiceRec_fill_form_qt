QT       += core gui
QT       += websockets
QT       += multimedia multimediawidgets
QT       += axcontainer
QT       += core5compat
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    TextProcessor.cpp \
    VoiceRecorder.cpp \
    VoiceRecorderThread.cpp \
    ccopy_tablewidget.cpp \
    col_name_mode_widget.cpp \
    configdialog.cpp \
    main.cpp \
    mainwindow.cpp \
    webiatws.cpp

HEADERS += \
    TextProcessor.h \
    VoiceRecorder.h \
    VoiceRecorderThread.h \
    ccopy_tablewidget.h \
    col_name_mode_widget.h \
    configdialog.h \
    copy_tablewidget.h \
    copy_tablewidget.h \
    copy_tablewidget.h \
    copy_tablewidget.h \
    copy_tablewidget.h \
    mainwindow.h \
    webiatws.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DESTDIR = $$PWD/../VoiceSearch_qt

LIBS += -LG:/voice/pinyin/libs/ -lpython38

INCLUDEPATH += G:/voice/pinyin/include
# DEPENDPATH += G:/voice/pinyin/include

win32:!win32-g++: PRE_TARGETDEPS += G:/voice/pinyin/libs/python38.lib

