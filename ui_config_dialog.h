/********************************************************************************
** Form generated from reading UI file 'config_dialog.ui'
**
** Created: Fri Aug 17 09:18:04 2012
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
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_config_dialog
{
public:
    QGridLayout *gridLayout_2;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QLabel *label;
    QComboBox *icon_show_combo;
    QLabel *label_2;
    QComboBox *icon_size_combo;
    QLabel *label_3;
    QComboBox *language_combo;
    QDialogButtonBox *buttonBox;
    QSpacerItem *verticalSpacer;
    QCheckBox *fluid_check;

    void setupUi(QDialog *config_dialog)
    {
        if (config_dialog->objectName().isEmpty())
            config_dialog->setObjectName(QString::fromUtf8("config_dialog"));
        config_dialog->resize(400, 300);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(config_dialog->sizePolicy().hasHeightForWidth());
        config_dialog->setSizePolicy(sizePolicy);
        gridLayout_2 = new QGridLayout(config_dialog);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        groupBox = new QGroupBox(config_dialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy1);
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        icon_show_combo = new QComboBox(groupBox);
        icon_show_combo->setObjectName(QString::fromUtf8("icon_show_combo"));

        gridLayout->addWidget(icon_show_combo, 0, 1, 1, 1);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        icon_size_combo = new QComboBox(groupBox);
        icon_size_combo->setObjectName(QString::fromUtf8("icon_size_combo"));

        gridLayout->addWidget(icon_size_combo, 1, 1, 1, 1);


        gridLayout_2->addWidget(groupBox, 0, 0, 1, 2);

        label_3 = new QLabel(config_dialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout_2->addWidget(label_3, 1, 0, 1, 1);

        language_combo = new QComboBox(config_dialog);
        language_combo->setObjectName(QString::fromUtf8("language_combo"));

        gridLayout_2->addWidget(language_combo, 1, 1, 1, 1);

        buttonBox = new QDialogButtonBox(config_dialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        gridLayout_2->addWidget(buttonBox, 4, 0, 1, 2);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer, 3, 0, 1, 2);

        fluid_check = new QCheckBox(config_dialog);
        fluid_check->setObjectName(QString::fromUtf8("fluid_check"));
        fluid_check->setChecked(true);

        gridLayout_2->addWidget(fluid_check, 2, 0, 1, 2);


        retranslateUi(config_dialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), config_dialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), config_dialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(config_dialog);
    } // setupUi

    void retranslateUi(QDialog *config_dialog)
    {
        config_dialog->setWindowTitle(QApplication::translate("config_dialog", "Dialog", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("config_dialog", "Toolbar icons", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("config_dialog", "Show", 0, QApplication::UnicodeUTF8));
        icon_show_combo->clear();
        icon_show_combo->insertItems(0, QStringList()
         << QApplication::translate("config_dialog", "Icon only", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("config_dialog", "Text only", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("config_dialog", "Icon besides text", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("config_dialog", "Icon below text", 0, QApplication::UnicodeUTF8)
        );
        label_2->setText(QApplication::translate("config_dialog", "Size", 0, QApplication::UnicodeUTF8));
        icon_size_combo->clear();
        icon_size_combo->insertItems(0, QStringList()
         << QApplication::translate("config_dialog", "Small (16x16)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("config_dialog", "Medium (24x24)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("config_dialog", "Large (48x48)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("config_dialog", "Huge (64x64)", 0, QApplication::UnicodeUTF8)
        );
        label_3->setText(QApplication::translate("config_dialog", "Language", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        fluid_check->setToolTip(QApplication::translate("config_dialog", "Enable Knot redraw while moving nodes", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        fluid_check->setText(QApplication::translate("config_dialog", "Fluid refresh", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class config_dialog: public Ui_config_dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONFIG_DIALOG_H
