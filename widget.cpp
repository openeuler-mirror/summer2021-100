#include "widget.h"
#include "ui_widget.h"

#include <QDebug>

Widget::Widget(QWidget *parent):
    QWidget(parent),
    ui(new Ui::schedule)
  , dateString("")
  , date(new QDate())
  , time(new QTime())
  , mTimerRefresh(new QTimer(this))
  , np(Q_NULLPTR)
  , m_settingsDatabase(Q_NULLPTR)
  , m_dbManager(Q_NULLPTR)
  , m_dbThread(Q_NULLPTR)
{
    /** 这里应为翻译部分代码
     *
     *
     *
     *
    **/

    ui->setupUi(this);

    //setupDatabases();
    kyScheduleInit();
    //kyScheduleConn();
    //QTimer::singleShot(200,this, SLOT(InitData()));




}

Widget::~Widget()
{
    delete ui;
}


void Widget::kyScheduleInit()
{
    //年月按钮初始化
    this->dateString.append(QString::number(this->ui->calendar->readSelectDate().year()));
    this->dateString.append("年");
    this->dateString.append(QString::number(this->ui->calendar->readSelectDate().month()));
    this->dateString.append("月");

    ui->yearButton->setText(dateString);

    //设置定时器 20ms一刷新
    mTimerRefresh->start(20);
    connect(mTimerRefresh, SIGNAL(timeout()), this, SLOT(widget_refresh()));
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
        this->dateString.clear();
        this->dateString.append(QString::number(this->ui->calendar->readSelectDate().year()));
        this->dateString.append("年");

        ui->yearButton->setText(dateString);
    }
    else{
        this->dateString.clear();
        this->dateString.append(QString::number(this->ui->calendar->readSelectDate().year()));
        this->dateString.append("年");
        this->dateString.append(QString::number(this->ui->calendar->readSelectDate().month()));
        this->dateString.append("月");

        ui->yearButton->setText(dateString);
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
