#include "weekcalendar.h"
#include <QDebug>

weekCalendar::weekCalendar(QWidget *parent):
    QWidget(parent)
    , m_isThemeChanged(false)
    , selected_col(-1)
{
    bgColor = QColor(170, 255, 255, 255);
    textColor = QColor(0, 0, 0, 255);
    shadowColor = QColor(0, 255, 255, 255);
    selectColor = QColor(0, 255, 255, 255);

    updateCalendar(this->selectDate);
}

weekCalendar::~weekCalendar()
{

}

void weekCalendar::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    //设置抗锯齿
    painter.setRenderHints(QPainter::Antialiasing);

    painter.setPen(Qt::NoPen);
    painter.fillRect(0, 0, 462, 470, bgColor);

    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.setPen(textColor);
    QString listHeadnum[7];

    for(int i = 0;i < 7;i++){
        listHeadnum[i].append(QString::number(this->dateItem[i].month));
        listHeadnum[i].append("月");
        listHeadnum[i].append(QString::number(this->dateItem[i].day));
        listHeadnum[i].append("日");

        if(dateItem[i].year == QDate::currentDate().year() &&
           dateItem[i].month == QDate::currentDate().month() &&
           dateItem[i].day == QDate::currentDate().day()){
            listHeadnum[i].clear();
            listHeadnum[i].append("今天");
        }

        if(dateItem[i].year == this->selectDate.year() &&
           dateItem[i].month == this->selectDate.month() &&
           dateItem[i].day == this->selectDate.day()){

            QRectF rectR = QRectF(66 * i, 0, 66, 470).adjusted(2, 2, -2 ,-2);
            painter.setBrush(Qt::NoBrush);
            painter.drawRoundedRect(rectR,4,4);
        }

    }

    for(int i = 0; i < 7; i++) {
        painter.drawText(i * 66, 0, 66, 25, Qt::AlignCenter, listHeadnum[i]);
    }

    QStringList listHead;

    listHead << "一" << "二" << "三" << "四" << "五" << "六" << "日";
    for(int i = 0; i < 7; i++) {
        painter.drawText(i * 66, 25, 66, 25, Qt::AlignCenter, listHead.at(i));
    }

    for(int i = 0;i < 7;i++){
        for(ScheduleData * schedule : this->dateItem[i].daily_ScheduleList){
            QTime stTime = schedule->startDateTime().time();
            QTime edTime = schedule->endDateTime().time();

            QRectF rect = QRectF(66 * i, 0.292 * (stTime.hour() * 60 + stTime.minute()) + 50, 66, 0.292 * ((edTime.hour()-stTime.hour()) * 60 + (edTime.minute() - stTime.minute()))).adjusted(5, 0, -5, 0);
            painter.setBrush(QBrush(textColor,Qt::SolidPattern));
            painter.drawRoundedRect(rect,2,2);
        }
    }

}

void weekCalendar::mousePressEvent(QMouseEvent *event){
    QPointF globalpoint = this->mapFromGlobal(QCursor::pos());

    if(event->button() == Qt::LeftButton){
            for (int column = 0; column < 7; column++) {
                QRectF rect = QRectF(66 * column, 0, 66, 470).adjusted(2, 2, -2, -2);
                if (rect.contains(globalpoint) && dateItem[column].day != -1) {
                    this->selectDate.setDate(this->selectDate.year(), this->selectDate.month(), this->dateItem[column].day);
                    mScheduleCheck = new monthScheduleCheck();
                    mScheduleCheck->scheduleListInit(QDate(dateItem[column].year, dateItem[column].month, dateItem[column].day),dateItem[column].daily_ScheduleList);
                    mScheduleCheck->setWindowModality(Qt::ApplicationModal);
                    connect(this->mScheduleCheck, &monthScheduleCheck::checkNewScheduleSignal, this, &weekCalendar::calendarNewScheduleSlots);
                    connect(this->mScheduleCheck, &monthScheduleCheck::checkInitScheduleSignal, this, &weekCalendar::calendarInitScheduleSlots);
                    connect(this->mScheduleCheck, &monthScheduleCheck::checkDeleteScheduleSignal, this, &weekCalendar::calendarDeleteScheduleSlots);
                    connect(this->mScheduleCheck, &monthScheduleCheck::checkUpdateScheduleSignal, this, &weekCalendar::calendarUpdateScheduleSlots);

                    this->selected_col = column;
                    mScheduleCheck->show();
                    mScheduleCheck->raise();
                    break;
                }
            }
    }
}

void weekCalendar::resetSelectDate(QDate date)
{
    setSelectDate(date);
}

void weekCalendar::updateCalendar(const QDate &selectDate)
{
    for(int i = 0;i < 7;i++){
        this->dateItem[i].year = -1;
        this->dateItem[i].month = -1;
        this->dateItem[i].day = -1;
    }

    int num_of_Week = selectDate.dayOfWeek();
    QDate theFirstDay = selectDate.addDays(-(num_of_Week - 1));

    for(int i = 0;i < 7;i++){
        this->dateItem[i].year = theFirstDay.year();
        this->dateItem[i].month = theFirstDay.month();
        this->dateItem[i].day = theFirstDay.day();

        theFirstDay = theFirstDay.addDays(1);
    }

    dateItemUpdate();
}

void weekCalendar::setSelectDate(const QDate &selectDate)
{
    this->selectDate = selectDate;
    updateCalendar(this->selectDate);
}


void weekCalendar::calendarNewScheduleSlots(ScheduleData* schedule)
{
    emit calendarNewScheduleSignal(schedule);
}

void weekCalendar::calendarInitScheduleSlots(ScheduleData* schedule)
{
    emit calendarInitScheduleSignal(schedule);
}

void weekCalendar::calendarDeleteScheduleSlots(ScheduleData* schedule)
{
    emit calendarDeleteScheduleSignal(schedule);
}

void weekCalendar::calendarUpdateScheduleSlots()
{
    this->mScheduleCheck->checkUpdate(this->dateItem[this->selected_col].daily_ScheduleList);
}

QDate weekCalendar::readSelectDate()
{
    return this->selectDate;
}

void weekCalendar::setBgColor(const QColor &bgcolor)
{
    this->bgColor = bgcolor;
}

void weekCalendar::setTextColor(const QColor &textcolor)
{
    this->textColor = textcolor;
}

void weekCalendar::setSelectColor(const QColor &selectColor)
{
    this->selectColor = selectColor;
}

void weekCalendar::setShadowColor(const QColor &shadowcolor)
{
    this->shadowColor = shadowcolor;
}

void weekCalendar::initDateItem(void)
{
    for(int i = 0;i < 7;i++){
            this->dateItem[i].daily_ScheduleList.clear();
    }
}

void weekCalendar::dateItemUpdate(void)
{
    initDateItem();
    for(ScheduleData* schedule : this->all_ScheduleList){
        for(int i = 0;i < 7;i++){
                        if(this->dateItem[i].year == schedule->startDateTime().date().year() &&
                           this->dateItem[i].month == schedule->startDateTime().date().month() &&
                           this->dateItem[i].day == schedule->startDateTime().date().day()){
                                if(!this->dateItem[i].daily_ScheduleList.contains(schedule))
                                    this->dateItem[i].daily_ScheduleList.append(schedule);
                                }
        }
    }

}

