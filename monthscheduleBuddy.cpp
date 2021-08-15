#include "monthscheduleBuddy.h"
#include "ui_monthscheduleBuddy.h"

#include <QDebug>

monthScheduleBuddy::monthScheduleBuddy(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::monthScheduleBuddy)
    , np(Q_NULLPTR)
{
    ui->setupUi(this);
    this->scheduleColor = QColor(46, 52, 54);
    this->ui->widget->setStyleSheet("QWidget{background-color: rgb(46, 52, 54);}");
    this->ui->label->setStyleSheet("font-size:30px;color:rgb(186, 189, 182);");
    this->ui->label_2->setStyleSheet("font-size:15px;color:rgb(186, 189, 182);");
}

monthScheduleBuddy::~monthScheduleBuddy()
{
    delete ui;
}

void monthScheduleBuddy::mouseDoubleClickEvent(QMouseEvent *event)
{
    this->np = new new_page();
    np->buddyUpdateInit(this->m_schedule);
    np->setWindowModality(Qt::ApplicationModal);
    connect(this->np, &new_page::sendScheduleData, this, &monthScheduleBuddy::buddyNewScheduleSlots);
    connect(this->np, &new_page::sendScheduleData, this, &monthScheduleBuddy::buddyInitScheduleSlots);
    connect(this->np, &new_page::checkNeedUpdateSignal,this, &monthScheduleBuddy::scheduleDeleteUpdateSlots);
    qDebug()<<"Buddy schedule ID";
    qDebug()<<this->m_schedule->id();
    np->show();
}

void monthScheduleBuddy::scheduleInit(ScheduleData *schedule)
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

void monthScheduleBuddy::scheduleDeleteUpdateSlots()
{
    emit scheduleDeleteUpdateSignal();
}

void monthScheduleBuddy::buddyNewScheduleSlots(ScheduleData* schedule)
{
    emit buddyNewScheduleSignal(schedule);
}
void monthScheduleBuddy::buddyInitScheduleSlots(ScheduleData* schedule)
{
    emit buddyInitScheduleSignal(schedule);
}

void monthScheduleBuddy::on_pushButton_clicked()
{
    emit scheduleDeleteSignal(this->m_schedule);
    QTimer::singleShot(200, this, SLOT(scheduleDeleteUpdateSlots()));
}
