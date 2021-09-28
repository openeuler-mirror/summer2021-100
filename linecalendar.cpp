#include "linecalendar.h"

lineCalendar::lineCalendar(QWidget *parent):
    QListWidget(parent)
    , m_isThemeChanged(false)
    , isToday(false)
{

}

lineCalendar::~lineCalendar()
{

}


bool sortCalendar(const ScheduleData* schedule1, const ScheduleData* schedule2)
{
    if(schedule1->startDateTime() < schedule2->startDateTime()){
        return true;
    }
    return false;
}

void lineCalendar::resetLineScroll()
{
    updateLineCalendar();
}

void lineCalendar::updateLineCalendar()
{
    this->clear();

    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    this->initDate.setDate(-1, -1, -1);

    qSort(this->all_ScheduleList.begin(), this->all_ScheduleList.end(), sortCalendar);

    for(ScheduleData* schedule : this->all_ScheduleList)
    {
        if(this->initDate != schedule->startDateTime().date()){
            this->initDate = schedule->startDateTime().date();
            QListWidgetItem *item1 = new QListWidgetItem();
            lineScheduleLabel *newLabelBuddy = new lineScheduleLabel();

            newLabelBuddy->initLabel(this->initDate);

            item1->setSizeHint(QSize(511, 60));
            this->addItem(item1);
            this->setItemWidget(item1, newLabelBuddy);

            if(this->initDate == QDate::currentDate()){
                this->isToday = true;
            }
        }

        QListWidgetItem *item2 = new QListWidgetItem();
        lineScheduleBuddy *newBuddy = new lineScheduleBuddy();
        newBuddy->scheduleInit(schedule);

        connect(newBuddy, &lineScheduleBuddy::scheduleDeleteSignal, this, &lineCalendar::checkDeleteScheduleSlots);
        connect(newBuddy, &lineScheduleBuddy::scheduleDeleteUpdateSignal, this, &lineCalendar::checkUpdateScheduleSlots);
        connect(newBuddy, &lineScheduleBuddy::scheduleDeleteSignal, this, &lineCalendar::checkInitScheduleSlots);

        connect(newBuddy, &lineScheduleBuddy::buddyNewScheduleSignal, this, &lineCalendar::checkNewScheduleSlots);
        connect(newBuddy, &lineScheduleBuddy::buddyInitScheduleSignal, this, &lineCalendar::checkInitScheduleSlots);

        item2->setSizeHint(QSize(511, 140));
        this->addItem(item2);
        this->setItemWidget(item2, newBuddy);

        if(schedule->startDateTime().date() == QDate::currentDate()){
            this->scrollToItem(item2);
        }
    }

    if(isToday == false){
        this->scrollToBottom();
    }
}

void lineCalendar::checkNewScheduleSlots(ScheduleData* schedule)
{
    emit checkNewScheduleSignal(schedule);
}

void lineCalendar::checkInitScheduleSlots(ScheduleData* schedule)
{
    emit checkInitScheduleSignal(schedule);
}

void lineCalendar::checkDeleteScheduleSlots(ScheduleData* schedule)
{
    emit checkDeleteScheduleSignal(schedule);
}

void lineCalendar::checkUpdateScheduleSlots()
{
    this->updateLineCalendar();
}


