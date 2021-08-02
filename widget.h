#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTimer>
#include <QTime>

#include "mainCalendar.h"

namespace Ui {
    class schedule;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    Ui::schedule *ui;   //主UI








private:
    QString date = "";  //显示当前日期的字符串
    QTime *time = new QTime();  //当前的时间
    QTimer *mTimerRefresh; // 画面刷新定时器 20ms一刷新

    void updateTimeButton();    //刷新时间显示
    void updateYearButton();    //刷新日期显示


private slots:
    void on_timerRefresh_timeout(); //定时器溢出处理 画面刷新 20ms
    void on_yearButton_clicked();   //点击年份显示按钮

signals:


};

#endif // WIDGET_H
