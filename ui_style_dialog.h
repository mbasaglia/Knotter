/********************************************************************************
** Form generated from reading UI file 'style_dialog.ui'
**
** Created: Fri Aug 17 13:33:38 2012
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
#include <QtGui/QTabWidget>
#include <QtGui/QWidget>
#include "color_widget.hpp"
#include "graphics/knotview.hpp"

QT_BEGIN_NAMESPACE

class Ui_StyleDialog
{
public:
    QGridLayout *gridLayout;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_2;
    KnotView *preview;
    QDialogButtonBox *buttonBox;
    QTabWidget *tabWidget;
    QWidget *tab;
    QGridLayout *gridLayout_3;
    QLabel *label;
    QDoubleSpinBox *outline_width_spinner;
    QDoubleSpinBox *knot_width_spinner;
    QLabel *label_2;
    ColorWidget *color;
    QLabel *label_3;
    QLabel *label_4;
    ColorWidget *outline_color;
    QLabel *label_10;
    QComboBox *outline_pattern_combo;
    QWidget *tab_2;
    QGridLayout *gridLayout_4;
    QLabel *label_5;
    QLabel *label_8;
    QLabel *label_7;
    QLabel *label_9;
    QDoubleSpinBox *handle_length_spinner;
    QDoubleSpinBox *crossing_gap_spinner;
    QDoubleSpinBox *cusp_angle_spinner;
    QLabel *label_6;
    QComboBox *style_combo;
    QComboBox *point_combo;

    void setupUi(QDialog *StyleDialog)
    {
        if (StyleDialog->objectName().isEmpty())
            StyleDialog->setObjectName(QString::fromUtf8("StyleDialog"));
        StyleDialog->resize(399, 420);
        gridLayout = new QGridLayout(StyleDialog);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
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


        gridLayout->addWidget(groupBox, 2, 0, 1, 1);

        buttonBox = new QDialogButtonBox(StyleDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Apply|QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        gridLayout->addWidget(buttonBox, 3, 0, 1, 1);

        tabWidget = new QTabWidget(StyleDialog);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        gridLayout_3 = new QGridLayout(tab);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        label = new QLabel(tab);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout_3->addWidget(label, 0, 0, 1, 1);

        outline_width_spinner = new QDoubleSpinBox(tab);
        outline_width_spinner->setObjectName(QString::fromUtf8("outline_width_spinner"));
        outline_width_spinner->setMinimum(1);
        outline_width_spinner->setMaximum(128);
        outline_width_spinner->setValue(1);

        gridLayout_3->addWidget(outline_width_spinner, 2, 1, 1, 1);

        knot_width_spinner = new QDoubleSpinBox(tab);
        knot_width_spinner->setObjectName(QString::fromUtf8("knot_width_spinner"));
        knot_width_spinner->setMinimum(1);
        knot_width_spinner->setValue(5);

        gridLayout_3->addWidget(knot_width_spinner, 0, 1, 1, 1);

        label_2 = new QLabel(tab);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout_3->addWidget(label_2, 2, 0, 1, 1);

        color = new ColorWidget(tab);
        color->setObjectName(QString::fromUtf8("color"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(color->sizePolicy().hasHeightForWidth());
        color->setSizePolicy(sizePolicy1);

        gridLayout_3->addWidget(color, 1, 1, 1, 1);

        label_3 = new QLabel(tab);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout_3->addWidget(label_3, 1, 0, 1, 1);

        label_4 = new QLabel(tab);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout_3->addWidget(label_4, 3, 0, 1, 1);

        outline_color = new ColorWidget(tab);
        outline_color->setObjectName(QString::fromUtf8("outline_color"));
        sizePolicy1.setHeightForWidth(outline_color->sizePolicy().hasHeightForWidth());
        outline_color->setSizePolicy(sizePolicy1);

        gridLayout_3->addWidget(outline_color, 3, 1, 1, 1);

        label_10 = new QLabel(tab);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        gridLayout_3->addWidget(label_10, 4, 0, 1, 1);

        outline_pattern_combo = new QComboBox(tab);
        outline_pattern_combo->setObjectName(QString::fromUtf8("outline_pattern_combo"));

        gridLayout_3->addWidget(outline_pattern_combo, 4, 1, 1, 1);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        gridLayout_4 = new QGridLayout(tab_2);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        label_5 = new QLabel(tab_2);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout_4->addWidget(label_5, 0, 0, 1, 1);

        label_8 = new QLabel(tab_2);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        gridLayout_4->addWidget(label_8, 1, 0, 1, 1);

        label_7 = new QLabel(tab_2);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout_4->addWidget(label_7, 4, 0, 1, 1);

        label_9 = new QLabel(tab_2);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        gridLayout_4->addWidget(label_9, 5, 0, 1, 1);

        handle_length_spinner = new QDoubleSpinBox(tab_2);
        handle_length_spinner->setObjectName(QString::fromUtf8("handle_length_spinner"));
        handle_length_spinner->setValue(32);

        gridLayout_4->addWidget(handle_length_spinner, 0, 1, 1, 1);

        crossing_gap_spinner = new QDoubleSpinBox(tab_2);
        crossing_gap_spinner->setObjectName(QString::fromUtf8("crossing_gap_spinner"));
        crossing_gap_spinner->setValue(10);

        gridLayout_4->addWidget(crossing_gap_spinner, 1, 1, 1, 1);

        cusp_angle_spinner = new QDoubleSpinBox(tab_2);
        cusp_angle_spinner->setObjectName(QString::fromUtf8("cusp_angle_spinner"));
        cusp_angle_spinner->setMaximum(360);
        cusp_angle_spinner->setSingleStep(15);
        cusp_angle_spinner->setValue(225);

        gridLayout_4->addWidget(cusp_angle_spinner, 2, 1, 1, 1);

        label_6 = new QLabel(tab_2);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout_4->addWidget(label_6, 2, 0, 1, 1);

        style_combo = new QComboBox(tab_2);
        style_combo->setObjectName(QString::fromUtf8("style_combo"));

        gridLayout_4->addWidget(style_combo, 4, 1, 1, 1);

        point_combo = new QComboBox(tab_2);
        point_combo->setObjectName(QString::fromUtf8("point_combo"));

        gridLayout_4->addWidget(point_combo, 5, 1, 1, 1);

        tabWidget->addTab(tab_2, QString());

        gridLayout->addWidget(tabWidget, 1, 0, 1, 1);


        retranslateUi(StyleDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), StyleDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), StyleDialog, SLOT(reject()));
        QObject::connect(buttonBox, SIGNAL(clicked(QAbstractButton*)), StyleDialog, SLOT(clicked(QAbstractButton*)));

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(StyleDialog);
    } // setupUi

    void retranslateUi(QDialog *StyleDialog)
    {
        StyleDialog->setWindowTitle(QApplication::translate("StyleDialog", "Knot Style", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("StyleDialog", "Preview", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("StyleDialog", "Knot Width", 0, QApplication::UnicodeUTF8));
        outline_width_spinner->setSuffix(QApplication::translate("StyleDialog", "px", 0, QApplication::UnicodeUTF8));
        knot_width_spinner->setSuffix(QApplication::translate("StyleDialog", "px", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("StyleDialog", "Outline Width", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("StyleDialog", "Knot Color", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("StyleDialog", "Outline Color", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("StyleDialog", "Outline Pattern", 0, QApplication::UnicodeUTF8));
        outline_pattern_combo->clear();
        outline_pattern_combo->insertItems(0, QStringList()
         << QApplication::translate("StyleDialog", "Solid", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("StyleDialog", "No Line", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("StyleDialog", "Dotted", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("StyleDialog", "Dashed", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("StyleDialog", "Dash Dot", 0, QApplication::UnicodeUTF8)
        );
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("StyleDialog", "Brush", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("StyleDialog", "Curve Control Point Distance", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("StyleDialog", "Crossing Gap", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("StyleDialog", "Curve Style", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("StyleDialog", "Point Style", 0, QApplication::UnicodeUTF8));
        handle_length_spinner->setSuffix(QApplication::translate("StyleDialog", "px", 0, QApplication::UnicodeUTF8));
        crossing_gap_spinner->setSuffix(QApplication::translate("StyleDialog", "px", 0, QApplication::UnicodeUTF8));
        cusp_angle_spinner->setSuffix(QApplication::translate("StyleDialog", "\302\260", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("StyleDialog", "Minimum Cusp Angle", 0, QApplication::UnicodeUTF8));
        style_combo->clear();
        style_combo->insertItems(0, QStringList()
         << QApplication::translate("StyleDialog", "Pointed Curve", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("StyleDialog", "Ogee", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("StyleDialog", "Pointed Polygonal", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("StyleDialog", "Polygonal", 0, QApplication::UnicodeUTF8)
        );
        point_combo->clear();
        point_combo->insertItems(0, QStringList()
         << QApplication::translate("StyleDialog", "Bevel", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("StyleDialog", "Miter", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("StyleDialog", "Round", 0, QApplication::UnicodeUTF8)
        );
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("StyleDialog", "Shape", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class StyleDialog: public Ui_StyleDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STYLE_DIALOG_H
