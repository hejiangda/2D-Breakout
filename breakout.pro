QT = core gui
TEMPLATE = app
HEADERS += abstractglscene.h     sceneglwindow.h \
    game.h \
    resourcemanager.h
SOURCES += main.cpp abstractglscene.cpp     sceneglwindow.cpp \
    game.cpp \
    resourcemanager.cpp

CONFIG += c++11

