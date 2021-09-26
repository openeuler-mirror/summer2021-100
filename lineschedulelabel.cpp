#include "lineschedulelabel.h"
#include "ui_lineschedulelabel.h"

lineScheduleLabel::lineScheduleLabel(QWidget *parent):
    QWidget(parent)
    , ui(new Ui::lineScheduleLabel)
{
    ui->setupUi(this);
}

lineScheduleLabel::~lineScheduleLabel()
{

}

void lineScheduleLabel::initLabel(QDate date)
{
    QString dateString;
    if(date == QDate::currentDate()){
        dateString.append("今日");
    }
    dateString.append(QString::number(date.month()));
    dateString.append("月");
    dateString.append(QString::number(date.day()));
    dateString.append("日");

    this->ui->label->setText(dateString);
}
