/********************************************************************************
** Form generated from reading UI file 'config_dialog.ui'
**
** Created: Fri Aug 17 16:47:38 2012
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
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_3;
    QCheckBox *fluid_check;
    QLabel *label_4;
    QComboBox *comboBox;

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

        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setEnabled(false);

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        language_combo = new QComboBox(groupBox);
        language_combo->setObjectName(QString::fromUtf8("language_combo"));

        gridLayout->addWidget(language_combo, 2, 1, 1, 1);


        gridLayout_2->addWidget(groupBox, 0, 0, 1, 1);

        buttonBox = new QDialogButtonBox(config_dialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        gridLayout_2->addWidget(buttonBox, 3, 0, 1, 1);

        groupBox_2 = new QGroupBox(config_dialog);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        gridLayout_3 = new QGridLayout(groupBox_2);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        fluid_check = new QCheckBox(groupBox_2);
        fluid_check->setObjectName(QString::fromUtf8("fluid_check"));
        fluid_check->setEnabled(false);
        fluid_check->setChecked(true);

        gridLayout_3->addWidget(fluid_check, 1, 0, 1, 2);

        label_4 = new QLabel(groupBox_2);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout_3->addWidget(label_4, 0, 0, 1, 1);

        comboBox = new QComboBox(groupBox_2);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));

        gridLayout_3->addWidget(comboBox, 0, 1, 1, 1);


        gridLayout_2->addWidget(groupBox_2, 2, 0, 1, 1);


        retranslateUi(config_dialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), config_dialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), config_dialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(config_dialog);
    } // setupUi

    void retranslateUi(QDialog *config_dialog)
    {
        config_dialog->setWindowTitle(QApplication::translate("config_dialog", "Dialog", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("config_dialog", "Interface", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("config_dialog", "Show", 0, QApplication::UnicodeUTF8));
        icon_show_combo->clear();
        icon_show_combo->insertItems(0, QStringList()
         << QApplication::translate("config_dialog", "Icon only", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("config_dialog", "Text only", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("config_dialog", "Icon besides text", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("config_dialog", "Icon below text", 0, QApplication::UnicodeUTF8)
        );
        label_2->setText(QApplication::translate("config_dialog", "Icon Size", 0, QApplication::UnicodeUTF8));
        icon_size_combo->clear();
        icon_size_combo->insertItems(0, QStringList()
         << QApplication::translate("config_dialog", "Small (16x16)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("config_dialog", "Medium (24x24)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("config_dialog", "Large (48x48)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("config_dialog", "Huge (64x64)", 0, QApplication::UnicodeUTF8)
        );
        label_3->setText(QApplication::translate("config_dialog", "Language", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("config_dialog", "Editing", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        fluid_check->setToolTip(QApplication::translate("config_dialog", "Enable Knot redraw while moving nodes", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        fluid_check->setText(QApplication::translate("config_dialog", "Fluid refresh", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("config_dialog", "Symmetric Editing", 0, QApplication::UnicodeUTF8));
        comboBox->clear();
        comboBox->insertItems(0, QStringList()
         << QApplication::translate("config_dialog", "Disabled", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("config_dialog", "Horizontal", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("config_dialog", "Vertical", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("config_dialog", "Quadrant", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("config_dialog", "Central", 0, QApplication::UnicodeUTF8)
        );
    } // retranslateUi

};

namespace Ui {
    class config_dialog: public Ui_config_dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONFIG_DIALOG_H
