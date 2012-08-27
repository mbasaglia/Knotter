

QT += svg


OTHER_FILES += \
    src/dialogs/README

HEADERS += \
    src/dialogs/knot_window.hpp \
    src/dialogs/gridconfig.hpp \
    src/dialogs/config_dialog.hpp \
    src/dialogs/export_dialog.hpp \
    src/dialogs/node_style_form.hpp \
    src/dialogs/global_style_form.hpp \
    src/dialogs/node_pref_dialog.hpp \
    src/dialogs/help_view.hpp

SOURCES += \
    src/dialogs/knot_window.cpp \
    src/dialogs/gridconfig.cpp \
    src/dialogs/config_dialog.cpp \
    src/dialogs/export_dialog.cpp \
    src/dialogs/node_style_form.cpp \
    src/dialogs/global_style_form.cpp \
    src/dialogs/node_pref_dialog.cpp \
    src/dialogs/help_view.cpp

FORMS += \
    src/dialogs/knot_window.ui \
    src/dialogs/gridconfig.ui \
    src/dialogs/config_dialog.ui \
    src/dialogs/export_dialog.ui \
    src/dialogs/node_style_form.ui \
    src/dialogs/global_style_form.ui \
    src/dialogs/node_pref_dialog.ui \
    src/dialogs/help_view.ui


INCLUDEPATH += src/dialogs

contains(WEBKIT,no) {
    DEFINES += NO_WEBKIT
    message("Webkit support is DISABLED")
} else {
    QT += webkit
}
