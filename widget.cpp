#include "widget.h"
#include "ui_widget.h"

#include <QDebug>

Widget::Widget(QWidget *parent):
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
    mTimerRefresh = new QTimer(this);
    mTimerRefresh->start(20);
    connect(mTimerRefresh, SIGNAL(timeout()), this, SLOT(widget_refresh()));


}

Widget::~Widget()
{
    delete ui;
}

//刷新时间显示
void Widget::updateTimeButton()
{
    this->ui->timeButton->setText(this->time->currentTime().toString("hh:mm:ss"));

}

//刷新日期显示
void Widget::updateYearButton()
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

//定时器溢出处理 画面刷新 20ms
void Widget::widget_refresh()
{
    this->ui->calendar->update();
    this->updateTimeButton();
    this->updateYearButton();
}

//点击年份显示按钮
void Widget::on_yearButton_clicked()
{
    this->ui->calendar->setPageState();

    updateYearButton();
}

//点击新建按钮
void Widget::on_newButton_clicked()
{
    np = new new_page;

    np->setWindowModality(Qt::ApplicationModal);
    np->show();
}
