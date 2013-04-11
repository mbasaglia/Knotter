 
INCLUDEPATH += $$PWD

include($$PWD/widgets/widgets.pri)
include($$PWD/dialogs/dialogs.pri)
include($$PWD/graph/graph.pri)
include($$PWD/scripting/scripting.pri)


SOURCES += \
    $$PWD/main.cpp \
    $$PWD/resource_manager.cpp \
    src/settings.cpp \
    src/string_toolbar.cpp

HEADERS += \
    $$PWD/resource_manager.hpp \
    src/settings.hpp \
    src/string_toolbar.hpp
