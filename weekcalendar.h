#ifndef WEEKCALENDAR_H
#define WEEKCALENDAR_H

#include <QObject>
#include <QWidget>
#include <QDate>
#include <QPainter>
#include <QMouseEvent>

#include "scheduledata.h"
#include "monthScheduleCheck.h"
#include "dateitem.h"

class weekCalendar : public QWidget
{
    Q_OBJECT
public:
    explicit weekCalendar(QWidget *parent = nullptr);
    ~weekCalendar();

    QList<ScheduleData *> all_ScheduleList;

    void updateCalendar(const QDate &selectDate);   //更新日历表
    void setBgColor(const QColor &bgcolor);
    void setTextColor(const QColor &textcolor);
    void setShadowColor(const QColor &shadowcolor);
    void setSelectColor(const QColor &selectColor);

    void initDateItem(void);

    void resetSelectDate(QDate date);

    QDate readSelectDate();     //返回选中日期

    DateItem dateItem[7];    //日期表
    bool m_isThemeChanged;          //主题色改变
    void dateItemUpdate();

protected:
    void paintEvent(QPaintEvent *);

private:
    QColor bgColor;
    QColor textColor;
    QColor shadowColor;
    QColor selectColor;

    QDate selectDate = QDate::currentDate();    //选中的日期
    QDate today = QDate::currentDate();         //今天的日期

    int selected_col;

    monthScheduleCheck* mScheduleCheck;

private slots:

    void setSelectDate(const QDate &selectDate);    //设置选中日期
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

#endif // WEEKCALENDAR_H
