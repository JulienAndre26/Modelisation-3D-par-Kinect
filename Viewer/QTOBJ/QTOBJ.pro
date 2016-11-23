QT += 3dcore 3drender 3dinput 3dextras

TEMPLATE = app


HEADERS += \
    planeentity.h \
    renderableentity.h \

SOURCES += main.cpp \
    planeentity.cpp \
    renderableentity.cpp \

RESOURCES += \
    ../exampleresources/chest.qrc \
    ../exampleresources/houseplants.qrc \
    ../exampleresources/metalbarrel.qrc \
    ../exampleresources/obj.qrc \
    ../exampleresources/textures.qrc
