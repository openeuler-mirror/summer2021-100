#include "new_page.h"
#include "ui_new_page.h"

#include <QDebug>

new_page::new_page(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::new_page)
    , this_page_data(Q_NULLPTR)
{
    ui->setupUi(this);

    this_page_data = new ScheduleData;

    this_page_data->setScheduleColor(0);
    this_page_data->setLastModificationDateTime(QDateTime::currentDateTime());
    this_page_data->setCreationDateTime(QDateTime::currentDateTime());
    this_page_data->setStartDateTime(QDateTime::currentDateTime());
    this_page_data->setEndDateTime(QDateTime::currentDateTime());


    ui->date_Edit->setDate(QDate::currentDate());
    ui->time_start_Edit->setTime(QTime::currentTime());
    ui->time_end_Edit->setTime(QTime::currentTime());

    connect(ui->yes_Button, &QPushButton::clicked, this, &new_page::onScheduleDataCreated);
}

new_page::~new_page()
{
    delete ui;
}

void new_page::on_no_Button_clicked()
{
    this->close();
}

void new_page::onScheduleDataCreated()
{
    QString now_datetime = QDate::currentDate().toString("yy-MM-dd") + " " + QTime::currentTime().toString("hh-mm-ss");
    QString start_datetime = ui->date_Edit->date().toString("yy-MM-dd") + " " + ui->time_start_Edit->time().toString("hh-mm-ss");
    QString end_datetime = ui->date_Edit->date().toString("yy-MM-dd") + " " + ui->time_end_Edit->time().toString("hh-mm-ss");
    this_page_data->setScheduleColor(0);
    this_page_data->setLastModificationDateTime(QDateTime::fromString(now_datetime, "yy-MM-dd hh-mm-ss"));
    this_page_data->setCreationDateTime(QDateTime::fromString(now_datetime, "yy-MM-dd hh-mm-ss"));
    this_page_data->setStartDateTime(QDateTime::fromString(start_datetime, "yy-MM-dd hh-mm-ss"));
    this_page_data->setEndDateTime(QDateTime::fromString(end_datetime, "yy-MM-dd hh-mm-ss"));
    this_page_data->setContent(ui->text_Edit->toPlainText());
    emit sendScheduleData(this_page_data);

    this->close();
}
