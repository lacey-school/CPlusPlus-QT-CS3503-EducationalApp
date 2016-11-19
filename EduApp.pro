#-------------------------------------------------
#
# Project created by QtCreator 2016-11-15T18:38:46
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = EduApp
TEMPLATE = app
QTPLUGIN =

SOURCES += \
    src/Main.cpp \
    src/QSFMLWidget.cpp \
    src/SFMLCanvas.cpp \
    src/ResourceManager.inl \
    src/Application.cpp \
    src/State.cpp \
    src/StateStack.cpp \
    src/TitleState.cpp \
    src/mysqlconnection.cpp

HEADERS += \
    src/QSFMLWidget.h \
    src/SFMLCanvas.h \
    src/ResourceManager.h \
    src/ResourceIdentifiers.h \
    src/Application.h \
    src/State.h \
    src/StateStack.h \
    src/StateIdentifiers.h \
    src/TitleState.h \
    src/mysqlconnection.h

FORMS += \
    src/Application.ui \
    src/TitleState.ui


!macx{
CONFIG(debug, debug|release): LIBS += -L$$PWD/ext/SFML/debug/lib
CONFIG(release, debug|release): LIBS += -L$$PWD/ext/SFML/release/lib

CONFIG(debug, debug|release): LIBS += -lsfml-audio-d -lsfml-graphics-d -lsfml-main-d -lsfml-network-d -lsfml-window-d -lsfml-system-d
CONFIG(release, debug|release): LIBS += -lsfml-audio -lsfml-graphics -lsfml-main -lsfml-network -lsfml-window -lsfml-system

INCLUDEPATH += $$PWD/ext/SFML/include
DEPENDPATH += $$PWD/ext/SFML/include
}

macx{
    LIBS += -L$$PWD/ext/SFML-2.4.1-osx-clang/lib -lsfml-audio -lsfml-graphics -lsfml-network -lsfml-window -lsfml-system

    INCLUDEPATH += $$PWD/ext/SFML-2.4.1-osx-clang/include
    DEPENDPATH += $$PWD/ext/SFML-2.4.1-osx-clang/include

    QMAKE_+= $$PWD/ext/SFML-2.4.1-osx-clang/lib -lsfml-audio -lsfml-graphics -lsfml-network -lsfml-window
}


!mac{
CONFIG(debug, debug|release): LIBS += -L$$PWD/ext/Box2D/debug -lBox2D
CONFIG(release, debug|release): LIBS += -L$$PWD/ext/Box2D/release -lBox2D

INCLUDEPATH += $$PWD/ext/Box2D
DEPENDPATH += $$PWD/ext/Box2D

}

macx{
LIBS += -L$$PWD/ext/Box2D/debug/ -lBox2D

INCLUDEPATH += $$PWD/ext/Box2D
DEPENDPATH += $$PWD/ext/Box2D

PRE_TARGETDEPS += $$PWD/ext/Box2D/debug/libBox2D.a
}


INCLUDEPATH += $$PWD/ext/MySQL/lib
DEPENDPATH += $$PWD/ext/MySQL/lib
#LIBS += $$PWD/ext/MySQL/lib -lmysqlclient -lm -lz

DISTFILES += \
    ../../../../../Qt/5.7/msvc2015_64/plugins/sqldrivers/qsqlmysql.dll
