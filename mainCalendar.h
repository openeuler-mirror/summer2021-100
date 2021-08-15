#ifndef MAINCALENDAR_H
#define MAINCALENDAR_H

#include <QObject>
#include <QWidget>
#include <QDate>
#include <QPainter>
#include <QMouseEvent>

#include "scheduledata.h"
#include "monthScheduleCheck.h"

enum page_state {year_state, month_state, day_state};

typedef struct DateItem {               //日期管理表
    int year;
    int month;
    int day;
    QList<ScheduleData *> daily_ScheduleList;

    DateItem()
    {
        year = -1;
        month = -1;
        day = -1;
    }
}DateItem;

class MainCalendar : public QWidget
{

    Q_OBJECT

public:
    explicit MainCalendar(QWidget *parent = nullptr);
    ~MainCalendar();

    page_state STATE = day_state;   //日历 年 月 日选择



    void updateCalendar(const QDate &selectDate);   //更新日历表
    void setBgColor(const QColor &bgcolor);
    void setTextColor(const QColor &textcolor);
    void setShadowColor(const QColor &shadowcolor);
    void setSelectColor(const QColor &selectColor);

    void initDateItem(void);

    DateItem dateItem[6][7];    //日期表
    bool m_isThemeChanged;          //主题色改变

protected:
    void paintEvent(QPaintEvent *);


private:
    QColor bgColor;
    QColor textColor;
    QColor shadowColor;
    QColor selectColor;

    QDate selectDate = QDate::currentDate();    //选中的日期
    QDate today = QDate::currentDate();         //今天的日期

    int selected_row;
    int selected_col;

    int yearSelect[3][3];       //年份表
    int monthSelect[3][4];      //月份表

    monthScheduleCheck* mScheduleCheck;



public slots:
    QDate readSelectDate();     //返回选中日期
    QString readPageState();    //返回日历状态
    void setSelectDate(const QDate &selectDate);    //设置选中日期
    void setPageState();                            //设置日历状态
    void mousePressEvent(QMouseEvent *event);

    void calendarNewScheduleSlots(ScheduleData* schedule);
    void calendarInitScheduleSlots(ScheduleData* schedule);
    void calendarDeleteScheduleSlots(ScheduleData* schedule);
    void calendarUpdateScheduleSlots();

signals:

    void calendarNewScheduleSignal(ScheduleData* schedule);
    void calendarInitScheduleSignal(ScheduleData* schedule);
    void calendarDeleteScheduleSignal(ScheduleData* schedule);


};

#endif // MAINCALENDAR_H
