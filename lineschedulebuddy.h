#ifndef LINESCHEDULEBUDDY_H
#define LINESCHEDULEBUDDY_H

#include <QWidget>
#include <QMouseEvent>
#include <QTimer>
#include "scheduledata.h"
#include "new_page.h"

namespace Ui {
class lineScheduleBuddy;
}

class lineScheduleBuddy : public QWidget
{
    Q_OBJECT
public:
    explicit lineScheduleBuddy(QWidget *parent = nullptr);
    ~lineScheduleBuddy();

    void scheduleInit(ScheduleData* schedule);

private:
    Ui::lineScheduleBuddy *ui;

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

#endif // LINESCHEDULEBUDDY_H
