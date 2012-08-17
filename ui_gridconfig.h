/********************************************************************************
** Form generated from reading UI file 'gridconfig.ui'
**
** Created: Fri Aug 17 09:18:04 2012
**      by: Qt User Interface Compiler version 4.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GRIDCONFIG_H
#define UI_GRIDCONFIG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_GridConfig
{
public:
    QGridLayout *gridLayout;
    QGroupBox *enable_check;
    QGridLayout *gridLayout_2;
    QLabel *label;
    QSpinBox *size_spin;
    QLabel *label_2;
    QComboBox *shape_combo;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *GridConfig)
    {
        if (GridConfig->objectName().isEmpty())
            GridConfig->setObjectName(QString::fromUtf8("GridConfig"));
        GridConfig->resize(400, 176);
        GridConfig->setModal(true);
        gridLayout = new QGridLayout(GridConfig);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        enable_check = new QGroupBox(GridConfig);
        enable_check->setObjectName(QString::fromUtf8("enable_check"));
        enable_check->setCheckable(true);
        enable_check->setChecked(true);
        gridLayout_2 = new QGridLayout(enable_check);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        label = new QLabel(enable_check);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout_2->addWidget(label, 0, 0, 1, 1);

        size_spin = new QSpinBox(enable_check);
        size_spin->setObjectName(QString::fromUtf8("size_spin"));
        size_spin->setMinimum(2);
        size_spin->setMaximum(1024);
        size_spin->setValue(64);

        gridLayout_2->addWidget(size_spin, 0, 1, 1, 1);

        label_2 = new QLabel(enable_check);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout_2->addWidget(label_2, 1, 0, 1, 1);

        shape_combo = new QComboBox(enable_check);
        shape_combo->setObjectName(QString::fromUtf8("shape_combo"));

        gridLayout_2->addWidget(shape_combo, 1, 1, 1, 1);


        gridLayout->addWidget(enable_check, 0, 0, 1, 1);

        buttonBox = new QDialogButtonBox(GridConfig);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        gridLayout->addWidget(buttonBox, 1, 0, 1, 1);


        retranslateUi(GridConfig);
        QObject::connect(buttonBox, SIGNAL(accepted()), GridConfig, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), GridConfig, SLOT(reject()));

        QMetaObject::connectSlotsByName(GridConfig);
    } // setupUi

    void retranslateUi(QDialog *GridConfig)
    {
        GridConfig->setWindowTitle(QApplication::translate("GridConfig", "Configure Grid", 0, QApplication::UnicodeUTF8));
        enable_check->setTitle(QApplication::translate("GridConfig", "Enable Grid", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("GridConfig", "Size", 0, QApplication::UnicodeUTF8));
        size_spin->setSuffix(QApplication::translate("GridConfig", "px", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("GridConfig", "Shape", 0, QApplication::UnicodeUTF8));
        shape_combo->clear();
        shape_combo->insertItems(0, QStringList()
         << QApplication::translate("GridConfig", "Square", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("GridConfig", "Triangle1", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("GridConfig", "Triangle2", 0, QApplication::UnicodeUTF8)
        );
    } // retranslateUi

};

namespace Ui {
    class GridConfig: public Ui_GridConfig {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GRIDCONFIG_H
