#include "schedule.h"
#include "ui_schedule.h"

#include <QDebug>

schedule::schedule(QWidget *parent):
    QWidget(parent),
    ui(new Ui::schedule)
{
    ui->setupUi(this);
}

schedule::~schedule()
{
    delete ui;
}


void schedule::on_pushButton_clicked()
{
    this->ui->calendar->setSelectDate(this->ui->dateEdit->date());
    this->ui->calendar->update();
}

