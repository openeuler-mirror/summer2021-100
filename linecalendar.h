#ifndef LINECALENDAR_H
#define LINECALENDAR_H

#include <QObject>
#include <QWidget>
#include <QListWidget>
#include <QDate>
#include <QPainter>
#include <QMouseEvent>

#include "scheduledata.h"
#include "lineschedulebuddy.h"
#include "lineschedulelabel.h"

class lineCalendar : public QListWidget
{
    Q_OBJECT
public:
    explicit lineCalendar(QWidget *parent = nullptr);
    ~lineCalendar();

public:
    bool m_isThemeChanged;          //主题色改变

    QList<ScheduleData *> all_ScheduleList;

    void updateLineCalendar();

private:

    QDate initDate;


private slots:


    void checkNewScheduleSlots(ScheduleData* schedule);
    void checkInitScheduleSlots(ScheduleData* schedule);
    void checkDeleteScheduleSlots(ScheduleData* schedule);
    void checkUpdateScheduleSlots();

signals:

    void checkNewScheduleSignal(ScheduleData* schedule);
    void checkInitScheduleSignal(ScheduleData* schedule);
    void checkDeleteScheduleSignal(ScheduleData* schedule);

};

#endif // LINECALENDAR_H
