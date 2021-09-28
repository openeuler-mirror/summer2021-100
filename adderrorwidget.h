#ifndef ADDERRORWIDGET_H
#define ADDERRORWIDGET_H

#include <QDialog>
#include <QPainter>
#include <QBitmap>
#include <QString>
#include "scheduledata.h"

namespace Ui {
class adderrorwidget;
}

class addErrorWidget : public QDialog
{
Q_OBJECT

public:
    explicit addErrorWidget(QWidget *parent=nullptr);
    ~addErrorWidget();

    int error_level = 0;

    ScheduleData * schedule;

    void update_ErrorWidget();

private slots:
    void on_pushButton_clicked();

    //void color_clicked();

private:
    Ui::adderrorwidget *ui;



    //void light_show();
    void black_show();

};

#endif // ADDERRORWIDGET_H
