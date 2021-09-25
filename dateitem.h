#ifndef DATEITEM_H
#define DATEITEM_H

#include "scheduledata.h"

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

#endif // DATEITEM_H
