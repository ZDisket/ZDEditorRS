#-------------------------------------------------
#
# Project created by QtCreator 2018-11-18T10:36:58
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ZDEditorRS
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

DEFINES += ZDFS_MAN_INCLUDE

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    ext/Qt-Frameless-Window-DarkStyle-master/DarkStyle.cpp \
    ext/Qt-Frameless-Window-DarkStyle-master/framelesswindow/framelesswindow.cpp \
    ext/Qt-Frameless-Window-DarkStyle-master/framelesswindow/windowdragger.cpp \
    generictablepage.cpp \
    ext/ZDFS.cpp \
    ext/ZFile.cpp \
    StrTable/GStringTable.cpp \
    StrTable/Stopwatch.cpp \
    ext/ZCharScanner.cpp \
    coleditdlg.cpp \
    newcoldlg.cpp \
    searchwindow.cpp \
    mulcoldlg.cpp

HEADERS += \
        mainwindow.h \
    ext/Delegate.h \
    ext/Signal.h \
    ext/Qt-Frameless-Window-DarkStyle-master/DarkStyle.h \
    ext/Qt-Frameless-Window-DarkStyle-master/framelesswindow/framelesswindow.h \
    ext/Qt-Frameless-Window-DarkStyle-master/framelesswindow/windowdragger.h \
    generictablepage.h \
    ext/ZDFS.h \
    ext/ZFile.h \
    StrTable/GStringTable.h \
    StrTable/Stopwatch.h \
    ext/ZCharScanner.h \
    macros.hpp \
    coleditdlg.h \
    newcoldlg.h \
    searchwindow.h \
    mulcoldlg.h

FORMS += \
        mainwindow.ui \
    ext/Qt-Frameless-Window-DarkStyle-master/framelesswindow/framelesswindow.ui \
    generictablepage.ui \
    coleditdlg.ui \
    newcoldlg.ui \
    searchwindow.ui \
    mulcoldlg.ui


INCLUDEPATH += $$PWD\ext\Qt-Frameless-Window-DarkStyle-master\framelesswindow
INCLUDEPATH += $$PWD\ext\Qt-Frameless-Window-DarkStyle-master\

# Link the database
INCLUDEPATH += $$PWD/DBI/include
DEPENDPATH += $$PWD/DBI/include
LIBS += -L"$$PWD/DBI/lib2"


RESOURCES += \
    ext/Qt-Frameless-Window-DarkStyle-master/framelesswindow.qrc \
    ext/Qt-Frameless-Window-DarkStyle-master/darkstyle.qrc \
    res/main.qrc

win32:RC_ICONS += ZDEditori.ico



