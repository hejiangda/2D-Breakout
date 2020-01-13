QT = core gui
TEMPLATE = app
HEADERS += abstractglscene.h     sceneglwindow.h \
    game.h \
    resourcemanager.h \
    spriterenderer.h
SOURCES += main.cpp abstractglscene.cpp     sceneglwindow.cpp \
    game.cpp \
    resourcemanager.cpp \
    spriterenderer.cpp

CONFIG += c++11

DISTFILES += \
    shaders/sprite.frag \
    shaders/sprite.vert

