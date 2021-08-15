#ifndef MONTHSCHEDULEBUDDY_H
#define MONTHSCHEDULEBUDDY_H

#include <QWidget>
#include <QMouseEvent>
#include <QTimer>
#include "scheduledata.h"
#include "new_page.h"

namespace Ui {
class monthScheduleBuddy;
}

class monthScheduleBuddy : public QWidget
{
    Q_OBJECT

public:
    explicit monthScheduleBuddy(QWidget *parent = nullptr);
    ~monthScheduleBuddy();

    void scheduleInit(ScheduleData* schedule);

private:
    Ui::monthScheduleBuddy *ui;

    QColor scheduleColor;

    ScheduleData *m_schedule;

    void mouseDoubleClickEvent(QMouseEvent* event);

    new_page* np;

private slots:

   void on_pushButton_clicked();

   void scheduleDeleteUpdateSlots();

   void buddyNewScheduleSlots(ScheduleData* schedule);
   void buddyInitScheduleSlots(ScheduleData* schedule);

signals:

   void scheduleDeleteSignal(ScheduleData* schedule);
   void scheduleDeleteUpdateSignal();

   void buddyNewScheduleSignal(ScheduleData* schedule);
   void buddyInitScheduleSignal(ScheduleData* schedule);

};

#endif // MONTHSCHEDULEBUDDY_H
