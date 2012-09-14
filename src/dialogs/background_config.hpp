#ifndef BACKGROUND_CONFIG_HPP
#define BACKGROUND_CONFIG_HPP

#include "ui_background_config.h"
#include "knotview.hpp"

class Background_Config : public QDialog, private Ui::Background_Config
{
        Q_OBJECT
    protected:
        KnotView * kv;
    public:
        explicit Background_Config( QWidget *parent = 0);

        void set_view ( KnotView * kview );
    private slots:
        void on_color_select_color_changed(const QColor &);
        void on_image_check_toggled(bool arg1);
        void on_file_name_editingFinished();
        void on_pushButton_clicked();
        void on_zoom_percent_valueChanged(double arg1);
};

#endif // BACKGROUND_CONFIG_HPP
