#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <QWidget>
#include <QTimer>
#include <QTime>

#include "maincalendar.h"

namespace Ui {
    class schedule;
}

class schedule : public QWidget
{
    Q_OBJECT

public:

    QTime *time = new QTime();
    QString date = "";

    explicit schedule(QWidget *parent = 0);
    ~schedule();

    QTimer *mTimer; // 设置定时器

private slots:

    void on_timer_timeout(); // 定时器溢出处理

    void on_yearButton_clicked();

private:
    void updateTimeButton();

    void updateYearButton();

    Ui::schedule *ui;
};

#endif // SCHEDULE_H
