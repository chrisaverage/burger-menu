CONFIG      += plugin debug_and_release

QT          += designer

TEMPLATE    = lib

TARGET      = burgermenu

HEADERS     = burgermenuplugin.h \
              ../src/burgermenu.h

SOURCES     = burgermenuplugin.cpp \
              ../src/burgermenu.cpp

INCLUDEPATH += ../src/

RESOURCES   = resource.qrc

target.path = $$[QT_INSTALL_PLUGINS]/designer

INSTALLS    += target
