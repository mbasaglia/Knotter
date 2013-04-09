 
INCLUDEPATH += $$PWD

include($$PWD/widgets/widgets.pri)
include($$PWD/dialogs/dialogs.pri)
include($$PWD/graph/graph.pri)
include($$PWD/scripting/scripting.pri)


SOURCES += \
    $$PWD/main.cpp \
    $$PWD/resource_manager.cpp

HEADERS += \
    $$PWD/resource_manager.hpp
