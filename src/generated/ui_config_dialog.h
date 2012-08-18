/********************************************************************************
** Form generated from reading UI file 'config_dialog.ui'
**
** Created: Sat Aug 18 17:22:13 2012
**      by: Qt User Interface Compiler version 4.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONFIG_DIALOG_H
#define UI_CONFIG_DIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_config_dialog
{
public:
    QGridLayout *gridLayout_2;
    QDialogButtonBox *buttonBox;
    QCheckBox *fluid_check;
    QLabel *label;
    QLabel *label_3;
    QComboBox *language_combo;
    QLabel *label_2;
    QComboBox *icon_size_combo;
    QLabel *label_4;
    QSpinBox *max_recent;
    QComboBox *icon_show_combo;
    QCheckBox *saveui_check;
    QPushButton *clear_recent;

    void setupUi(QDialog *config_dialog)
    {
        if (config_dialog->objectName().isEmpty())
            config_dialog->setObjectName(QString::fromUtf8("config_dialog"));
        config_dialog->resize(292, 222);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(config_dialog->sizePolicy().hasHeightForWidth());
        config_dialog->setSizePolicy(sizePolicy);
        gridLayout_2 = new QGridLayout(config_dialog);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        buttonBox = new QDialogButtonBox(config_dialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        gridLayout_2->addWidget(buttonBox, 9, 0, 1, 3);

        fluid_check = new QCheckBox(config_dialog);
        fluid_check->setObjectName(QString::fromUtf8("fluid_check"));
        fluid_check->setEnabled(false);
        fluid_check->setChecked(true);

        gridLayout_2->addWidget(fluid_check, 8, 0, 1, 3);

        label = new QLabel(config_dialog);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout_2->addWidget(label, 2, 0, 1, 1);

        label_3 = new QLabel(config_dialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setEnabled(false);

        gridLayout_2->addWidget(label_3, 6, 0, 1, 1);

        language_combo = new QComboBox(config_dialog);
        language_combo->setObjectName(QString::fromUtf8("language_combo"));

        gridLayout_2->addWidget(language_combo, 6, 1, 1, 2);

        label_2 = new QLabel(config_dialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout_2->addWidget(label_2, 0, 0, 1, 1);

        icon_size_combo = new QComboBox(config_dialog);
        icon_size_combo->setObjectName(QString::fromUtf8("icon_size_combo"));

        gridLayout_2->addWidget(icon_size_combo, 0, 1, 1, 2);

        label_4 = new QLabel(config_dialog);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout_2->addWidget(label_4, 4, 0, 1, 1);

        max_recent = new QSpinBox(config_dialog);
        max_recent->setObjectName(QString::fromUtf8("max_recent"));
        max_recent->setMaximum(128);
        max_recent->setValue(5);

        gridLayout_2->addWidget(max_recent, 4, 1, 1, 1);

        icon_show_combo = new QComboBox(config_dialog);
        icon_show_combo->setObjectName(QString::fromUtf8("icon_show_combo"));

        gridLayout_2->addWidget(icon_show_combo, 2, 1, 1, 2);

        saveui_check = new QCheckBox(config_dialog);
        saveui_check->setObjectName(QString::fromUtf8("saveui_check"));
        saveui_check->setChecked(true);

        gridLayout_2->addWidget(saveui_check, 7, 0, 1, 3);

        clear_recent = new QPushButton(config_dialog);
        clear_recent->setObjectName(QString::fromUtf8("clear_recent"));

        gridLayout_2->addWidget(clear_recent, 4, 2, 1, 1);


        retranslateUi(config_dialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), config_dialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), config_dialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(config_dialog);
    } // setupUi

    void retranslateUi(QDialog *config_dialog)
    {
        config_dialog->setWindowTitle(QApplication::translate("config_dialog", "Dialog", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        fluid_check->setToolTip(QApplication::translate("config_dialog", "Redraw the knot while moving nodes", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        fluid_check->setText(QApplication::translate("config_dialog", "Fluid refresh", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("config_dialog", "Toolbar Icons", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("config_dialog", "Language", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("config_dialog", "Toolbar Icon Size", 0, QApplication::UnicodeUTF8));
        icon_size_combo->clear();
        icon_size_combo->insertItems(0, QStringList()
         << QApplication::translate("config_dialog", "Small (16x16)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("config_dialog", "Medium (24x24)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("config_dialog", "Large (48x48)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("config_dialog", "Huge (64x64)", 0, QApplication::UnicodeUTF8)
        );
        label_4->setText(QApplication::translate("config_dialog", "Max recent files", 0, QApplication::UnicodeUTF8));
        icon_show_combo->clear();
        icon_show_combo->insertItems(0, QStringList()
         << QApplication::translate("config_dialog", "Icon only", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("config_dialog", "Text only", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("config_dialog", "Icon besides text", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("config_dialog", "Icon below text", 0, QApplication::UnicodeUTF8)
        );
        saveui_check->setText(QApplication::translate("config_dialog", "Save interface on exit", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        clear_recent->setToolTip(QApplication::translate("config_dialog", "Clear recent file history", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        clear_recent->setText(QApplication::translate("config_dialog", "Clear", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class config_dialog: public Ui_config_dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONFIG_DIALOG_H
