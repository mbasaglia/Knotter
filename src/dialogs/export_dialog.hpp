#ifndef EXPORT_DIALOG_HPP
#define EXPORT_DIALOG_HPP

#include "ui_export_dialog.h"
#include "knotview.hpp"

class Export_Dialog : public QDialog, private Ui::Export_Dialog
{
        Q_OBJECT

    protected:
        QString     filename;
        KnotView*   canvas;
        double      ratio; ///< height/width

        QSize get_size() const;
        QRectF get_area();
    public:
        explicit Export_Dialog( QWidget *parent = 0);

        void set_knot_view ( KnotView* kn );

    public slots:
        void reset_size();


    private slots:
        void on_export_svg_button_clicked();
        void on_export_raster_button_clicked();
        void on_quality_slider_valueChanged(int value);
        void on_width_spin_valueChanged(int arg1);
        void on_height_spin_valueChanged(int arg1);
        void on_ratio_check_toggled(bool checked);
};

#endif // EXPORT_DIALOG_HPP
