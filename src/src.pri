
SOURCES += src/main.cpp \
    src/error_recovery.cpp \
    src/context_menu.cpp \
    src/resource_loader.cpp \
    src/translator.cpp \
    src/extended_tabwidget.cpp


include(dialogs/dialogs.pri)
include(graphics/graphics.pri)

OTHER_FILES += \
    src/README \
    src/generated/README

HEADERS += \
    src/color_widget.hpp \
    src/error_recovery.hpp \
    src/context_menu.hpp \
    src/resource_loader.hpp \
    src/translator.hpp \
    src/extended_tabwidget.hpp

INCLUDEPATH += src
