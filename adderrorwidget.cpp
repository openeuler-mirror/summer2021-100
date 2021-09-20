#include "adderrorwidget.h"
#include "ui_adderrorwidget.h"

#include <QDebug>

addErrorWidget::addErrorWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::adderrorwidget)
{
    ui->setupUi(this);

    this->schedule = new ScheduleData;

    QBitmap bmp(this->size());
    bmp.fill();
    QPainter p(&bmp);
    p.setPen(Qt::NoPen);
    p.setBrush(Qt::black);
    p.setRenderHint(QPainter::Antialiasing);
    p.drawRoundedRect(bmp.rect(),6,6);
    setMask(bmp);

    this->setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(Qt::FramelessWindowHint);
}
addErrorWidget::~addErrorWidget()
{
    delete ui;
}

void addErrorWidget::on_pushButton_clicked()
{
    this->close();
}

void addErrorWidget::update_ErrorWidget()
{
    if(error_level == 1){
        this->ui->errorText->setText(QString("日程信息请勿填空"));
        this->ui->errorInfo->setText(QString(""));
    }else if(error_level == 2){
        this->ui->errorText->setText(QString("请确认日程结束时间是否早于日程开始时间"));
        this->ui->errorInfo->setText(QString(""));
    }else if(error_level == 3){
        QString infoText;
        infoText.append(QString::number(schedule->startDateTime().date().year()));
        infoText.append(QString("年"));
        infoText.append(QString::number(schedule->startDateTime().date().month()));
        infoText.append(QString("月"));
        infoText.append(QString::number(schedule->startDateTime().date().day()));
        infoText.append(QString("日"));
        infoText.append(QString(" "));
        infoText.append(QString::number(schedule->startDateTime().time().hour()));
        infoText.append(QString("时"));
        infoText.append(QString::number(schedule->startDateTime().time().minute()));
        infoText.append(QString("分"));
        infoText.append(QString("-"));
        infoText.append(QString::number(schedule->endDateTime().time().hour()));
        infoText.append(QString("时"));
        infoText.append(QString::number(schedule->endDateTime().time().minute()));
        infoText.append(QString("分"));
        infoText.append(QString(" "));
        infoText.append(schedule->content().midRef(0,4));
        if(schedule->content().length() > 5)
            infoText.append(QString("..."));
        this->ui->errorText->setText(QString("此日程与已有日程冲突:"));
        this->ui->errorInfo->setText(infoText);
    }
}
