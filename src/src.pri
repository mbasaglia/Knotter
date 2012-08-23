
SOURCES += src/main.cpp \
    src/error_recovery.cpp \
    src/context_menu.cpp \
    src/xml_saver.cpp \
    src/resource_loader.cpp


include(dialogs/dialogs.pri)
include(graphics/graphics.pri)

OTHER_FILES += \
    src/README \
    src/generated/README

HEADERS += \
    src/color_widget.hpp \
    src/error_recovery.hpp \
    src/context_menu.hpp \
    src/xml_saver.hpp \
    src/resource_loader.hpp

INCLUDEPATH += src
