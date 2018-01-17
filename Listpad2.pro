#-------------------------------------------------
#
# Project created by QtCreator 2018-01-13T10:38:05
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Listpad2
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    LogoWidget.cpp \
    MenuButton.cpp \
    Block.cpp \
    DesktopButton.cpp \
    Table.cpp \
    Menu.cpp \
    ScrollBar.cpp

HEADERS  += MainWindow.h \
    LogoWidget.h \
    MenuButton.h \
    Block.h \
    DesktopButton.h \
    Table.h \
    Menu.h \
    ScrollBar.h

FORMS    += MainWindow.ui \
    Table.ui \
    Menu.ui \
