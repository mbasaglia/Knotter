OTHER_FILES += \
    src/dialogs/README

HEADERS += \
    src/dialogs/style_dialog.hpp \
    src/dialogs/knot_window.hpp \
    src/dialogs/gridconfig.hpp \
    src/dialogs/config_dialog.hpp \
    src/dialogs/export_dialog.hpp

SOURCES += \
    src/dialogs/style_dialog.cpp \
    src/dialogs/knot_window.cpp \
    src/dialogs/gridconfig.cpp \
    src/dialogs/config_dialog.cpp \
    src/dialogs/export_dialog.cpp

FORMS += \
    src/dialogs/knot_window.ui \
    src/dialogs/gridconfig.ui \
    src/dialogs/config_dialog.ui \
    src/dialogs/style_dialog.ui \
    src/dialogs/export_dialog.ui


INCLUDEPATH += src/dialogs
