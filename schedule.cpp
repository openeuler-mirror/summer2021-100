#include "schedule.h"
#include "ui_schedule.h"

#include <QDebug>

schedule::schedule(QWidget *parent):
    QWidget(parent),
    ui(new Ui::schedule)
{
    ui->setupUi(this);

    //年月按钮初始化
    this->date.append(QString::number(this->ui->calendar->readSelectDate().year()));
    this->date.append("年");
    this->date.append(QString::number(this->ui->calendar->readSelectDate().month()));
    this->date.append("月");

    ui->yearButton->setText(date);

    //设置定时器 20ms一刷新
    mTimer = new QTimer(this);
    mTimer->stop();
    connect(mTimer, SIGNAL(timeout()), this, SLOT(on_timer_timeout()));
    mTimer->start(20);

}

schedule::~schedule()
{
    delete ui;
}

//刷新时间按钮
void schedule::updateTimeButton()
{
    this->ui->timeButton->setText(this->time->currentTime().toString("hh:mm:ss"));

}

void schedule::updateYearButton()
{
    if(this->ui->calendar->readPageState() == "month_select" || this->ui->calendar->readPageState() == "year_select"){
        this->date.clear();
        this->date.append(QString::number(this->ui->calendar->readSelectDate().year()));
        this->date.append("年");

        ui->yearButton->setText(date);
    }
    else{
        this->date.clear();
        this->date.append(QString::number(this->ui->calendar->readSelectDate().year()));
        this->date.append("年");
        this->date.append(QString::number(this->ui->calendar->readSelectDate().month()));
        this->date.append("月");

        ui->yearButton->setText(date);
    }
}
//定时刷新
void schedule::on_timer_timeout()
{
    this->ui->calendar->update();
    this->updateTimeButton();
    this->updateYearButton();
}


void schedule::on_yearButton_clicked()
{
    this->ui->calendar->setPageState();

    updateYearButton();
}

