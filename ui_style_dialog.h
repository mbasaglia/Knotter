/********************************************************************************
** Form generated from reading UI file 'style_dialog.ui'
**
** Created: Fri Aug 17 09:18:04 2012
**      by: Qt User Interface Compiler version 4.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STYLE_DIALOG_H
#define UI_STYLE_DIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include "color_widget.hpp"
#include "graphics/knotview.hpp"

QT_BEGIN_NAMESPACE

class Ui_StyleDialog
{
public:
    QGridLayout *gridLayout;
    QLabel *label;
    QDoubleSpinBox *knot_width_spinner;
    QLabel *label_2;
    QDoubleSpinBox *outline_width_spinner;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QDoubleSpinBox *handle_length_spinner;
    QDialogButtonBox *buttonBox;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_2;
    KnotView *preview;
    QLabel *label_6;
    QDoubleSpinBox *cusp_angle_spinner;
    ColorWidget *color;
    ColorWidget *outline_color;
    QLabel *label_7;
    QComboBox *style_combo;
    QLabel *label_8;
    QDoubleSpinBox *crossing_gap_spinner;
    QLabel *label_9;
    QComboBox *point_combo;

    void setupUi(QDialog *StyleDialog)
    {
        if (StyleDialog->objectName().isEmpty())
            StyleDialog->setObjectName(QString::fromUtf8("StyleDialog"));
        StyleDialog->resize(400, 485);
        gridLayout = new QGridLayout(StyleDialog);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(StyleDialog);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        knot_width_spinner = new QDoubleSpinBox(StyleDialog);
        knot_width_spinner->setObjectName(QString::fromUtf8("knot_width_spinner"));
        knot_width_spinner->setMinimum(1);
        knot_width_spinner->setValue(5);

        gridLayout->addWidget(knot_width_spinner, 0, 1, 1, 1);

        label_2 = new QLabel(StyleDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        outline_width_spinner = new QDoubleSpinBox(StyleDialog);
        outline_width_spinner->setObjectName(QString::fromUtf8("outline_width_spinner"));
        outline_width_spinner->setMaximum(128);
        outline_width_spinner->setValue(1);

        gridLayout->addWidget(outline_width_spinner, 1, 1, 1, 1);

        label_3 = new QLabel(StyleDialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        label_4 = new QLabel(StyleDialog);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 3, 0, 1, 1);

        label_5 = new QLabel(StyleDialog);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout->addWidget(label_5, 4, 0, 1, 1);

        handle_length_spinner = new QDoubleSpinBox(StyleDialog);
        handle_length_spinner->setObjectName(QString::fromUtf8("handle_length_spinner"));
        handle_length_spinner->setValue(32);

        gridLayout->addWidget(handle_length_spinner, 4, 1, 1, 1);

        buttonBox = new QDialogButtonBox(StyleDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Apply|QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        gridLayout->addWidget(buttonBox, 10, 0, 1, 2);

        groupBox = new QGroupBox(StyleDialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout_2 = new QGridLayout(groupBox);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        preview = new KnotView(groupBox);
        preview->setObjectName(QString::fromUtf8("preview"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(preview->sizePolicy().hasHeightForWidth());
        preview->setSizePolicy(sizePolicy);
        preview->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        preview->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        preview->setInteractive(false);

        gridLayout_2->addWidget(preview, 0, 0, 1, 1);


        gridLayout->addWidget(groupBox, 9, 0, 1, 2);

        label_6 = new QLabel(StyleDialog);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout->addWidget(label_6, 6, 0, 1, 1);

        cusp_angle_spinner = new QDoubleSpinBox(StyleDialog);
        cusp_angle_spinner->setObjectName(QString::fromUtf8("cusp_angle_spinner"));
        cusp_angle_spinner->setMaximum(360);
        cusp_angle_spinner->setSingleStep(15);
        cusp_angle_spinner->setValue(225);

        gridLayout->addWidget(cusp_angle_spinner, 6, 1, 1, 1);

        color = new ColorWidget(StyleDialog);
        color->setObjectName(QString::fromUtf8("color"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(color->sizePolicy().hasHeightForWidth());
        color->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(color, 2, 1, 1, 1);

        outline_color = new ColorWidget(StyleDialog);
        outline_color->setObjectName(QString::fromUtf8("outline_color"));
        sizePolicy1.setHeightForWidth(outline_color->sizePolicy().hasHeightForWidth());
        outline_color->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(outline_color, 3, 1, 1, 1);

        label_7 = new QLabel(StyleDialog);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout->addWidget(label_7, 7, 0, 1, 1);

        style_combo = new QComboBox(StyleDialog);
        style_combo->setObjectName(QString::fromUtf8("style_combo"));

        gridLayout->addWidget(style_combo, 7, 1, 1, 1);

        label_8 = new QLabel(StyleDialog);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        gridLayout->addWidget(label_8, 5, 0, 1, 1);

        crossing_gap_spinner = new QDoubleSpinBox(StyleDialog);
        crossing_gap_spinner->setObjectName(QString::fromUtf8("crossing_gap_spinner"));
        crossing_gap_spinner->setValue(10);

        gridLayout->addWidget(crossing_gap_spinner, 5, 1, 1, 1);

        label_9 = new QLabel(StyleDialog);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        gridLayout->addWidget(label_9, 8, 0, 1, 1);

        point_combo = new QComboBox(StyleDialog);
        point_combo->setObjectName(QString::fromUtf8("point_combo"));

        gridLayout->addWidget(point_combo, 8, 1, 1, 1);


        retranslateUi(StyleDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), StyleDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), StyleDialog, SLOT(reject()));
        QObject::connect(buttonBox, SIGNAL(clicked(QAbstractButton*)), StyleDialog, SLOT(clicked(QAbstractButton*)));

        QMetaObject::connectSlotsByName(StyleDialog);
    } // setupUi

    void retranslateUi(QDialog *StyleDialog)
    {
        StyleDialog->setWindowTitle(QApplication::translate("StyleDialog", "Knot Style", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("StyleDialog", "Knot Width", 0, QApplication::UnicodeUTF8));
        knot_width_spinner->setSuffix(QApplication::translate("StyleDialog", "px", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("StyleDialog", "Outline Width", 0, QApplication::UnicodeUTF8));
        outline_width_spinner->setSuffix(QApplication::translate("StyleDialog", "px", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("StyleDialog", "Color", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("StyleDialog", "Outline Color", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("StyleDialog", "Curve Control Point Distance", 0, QApplication::UnicodeUTF8));
        handle_length_spinner->setSuffix(QApplication::translate("StyleDialog", "px", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("StyleDialog", "Preview", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("StyleDialog", "Minimum Cusp Angle", 0, QApplication::UnicodeUTF8));
        cusp_angle_spinner->setSuffix(QApplication::translate("StyleDialog", "\302\260", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("StyleDialog", "Curve Style", 0, QApplication::UnicodeUTF8));
        style_combo->clear();
        style_combo->insertItems(0, QStringList()
         << QApplication::translate("StyleDialog", "Pointed Curve", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("StyleDialog", "Ogee", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("StyleDialog", "Pointed Polygonal", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("StyleDialog", "Polygonal", 0, QApplication::UnicodeUTF8)
        );
        label_8->setText(QApplication::translate("StyleDialog", "Crossing Gap", 0, QApplication::UnicodeUTF8));
        crossing_gap_spinner->setSuffix(QApplication::translate("StyleDialog", "px", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("StyleDialog", "Point Style", 0, QApplication::UnicodeUTF8));
        point_combo->clear();
        point_combo->insertItems(0, QStringList()
         << QApplication::translate("StyleDialog", "Bevel", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("StyleDialog", "Miter", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("StyleDialog", "Round", 0, QApplication::UnicodeUTF8)
        );
    } // retranslateUi

};

namespace Ui {
    class StyleDialog: public Ui_StyleDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STYLE_DIALOG_H
