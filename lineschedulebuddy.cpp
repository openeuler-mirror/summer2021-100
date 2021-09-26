#include "lineschedulebuddy.h"
#include "ui_lineschedulebuddy.h"

lineScheduleBuddy::lineScheduleBuddy(QWidget *parent):
    QWidget(parent)
    , ui(new Ui::lineScheduleBuddy)
{
    ui->setupUi(this);

    this->ui->widget->setStyleSheet("QWidget{background-color: rgb(46, 52, 54);}");
    this->ui->label->setStyleSheet("font-size:30px;color:rgb(186, 189, 182);");
    this->ui->label_2->setStyleSheet("font-size:15px;color:rgb(186, 189, 182);");
}

lineScheduleBuddy::~lineScheduleBuddy()
{
    delete ui;
}

void lineScheduleBuddy::mouseDoubleClickEvent(QMouseEvent *event)
{
    this->np = new new_page();
    np->setUpdate();
    np->buddyUpdateInit(this->m_schedule);
    np->setWindowModality(Qt::ApplicationModal);
    connect(this->np, &new_page::sendScheduleData, this, &lineScheduleBuddy::buddyNewScheduleSlots);
    connect(this->np, &new_page::sendScheduleData, this, &lineScheduleBuddy::buddyInitScheduleSlots);
    connect(this->np, &new_page::checkNeedUpdateSignal,this, &lineScheduleBuddy::scheduleDeleteUpdateSlots);
    np->show();
}

void lineScheduleBuddy::scheduleInit(ScheduleData *schedule)
{
    QString SEDate;
    QString Content;
    this->m_schedule = schedule;
    SEDate.append(m_schedule->startDateTime().time().toString("hh:mm"));
    SEDate.append(" - ");
    SEDate.append(m_schedule->endDateTime().time().toString("hh:mm"));
    Content.append(m_schedule->content());

    this->ui->label->setText(SEDate);
    this->ui->label_2->setText(Content);

}

void lineScheduleBuddy::scheduleDeleteUpdateSlots()
{
    emit scheduleDeleteUpdateSignal();
}

void lineScheduleBuddy::buddyNewScheduleSlots(ScheduleData* schedule)
{
    emit buddyNewScheduleSignal(schedule);
}
void lineScheduleBuddy::buddyInitScheduleSlots(ScheduleData* schedule)
{
    emit buddyInitScheduleSignal(schedule);
}

void lineScheduleBuddy::on_pushButton_clicked()
{
    emit scheduleDeleteSignal(this->m_schedule);
    QTimer::singleShot(200, this, SLOT(scheduleDeleteUpdateSlots()));
}
