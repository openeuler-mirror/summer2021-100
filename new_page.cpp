#include "new_page.h"
#include "ui_new_page.h"
#include "widget.h"
#include "ui_widget.h"

#include <QDebug>

new_page::new_page(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::new_page)
    , ifUpdate(false)
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
    ui->time_end_Edit->setTime(QTime::currentTime().addSecs(30 * 60));

    mTimerRefresh = new QTimer(this);

    mTimerRefresh->start(100);
    connect(mTimerRefresh, SIGNAL(timeout()), this, SLOT(label_refresh()));


    setWindowFlags (Qt::FramelessWindowHint);

    if(page_theme == false){
        black_show();
    }
    else{
        light_show();
    }


    connect(ui->yes_Button, &QPushButton::clicked, this, &new_page::onScheduleDataCreated);
}

new_page::~new_page()
{
    delete ui;
}

void new_page::checkNewInit(QDate date)
{
    ui->date_Edit->setDate(date);
    ui->date_Edit->setReadOnly(true);
}

void new_page::buddyUpdateInit(ScheduleData *schedule)
{
    this_page_data->setId(schedule->id());
    ui->date_Edit->setDate(schedule->startDateTime().date());
    ui->date_Edit->setReadOnly(true);

    ui->text_Edit->setText(schedule->content());
    qDebug()<<"SenderIn ID";
    qDebug()<<this_page_data->id();
}


void new_page::on_no_Button_clicked()
{
    this->close();
}

void new_page::onScheduleDataCreated()
{


    Widget *ptr = (Widget*)parentWidget();
    addErrorWidget *errorWidget = new addErrorWidget;
    errorWidget->setWindowModality(Qt::ApplicationModal);
    int error = 0;

    qDebug()<<"执行信号";
    QString now_datetime = QDate::currentDate().toString("yyyy-MM-dd") + " " + QTime::currentTime().toString("hh-mm-ss");
    QString start_datetime = ui->date_Edit->date().toString("yyyy-MM-dd") + " " + ui->time_start_Edit->time().toString("hh-mm-ss");
    QString end_datetime = ui->date_Edit->date().toString("yyyy-MM-dd") + " " + ui->time_end_Edit->time().toString("hh-mm-ss");
    this_page_data->setScheduleColor(0);
    this_page_data->setLastModificationDateTime(QDateTime::fromString(now_datetime, "yyyy-MM-dd hh-mm-ss"));
    this_page_data->setCreationDateTime(QDateTime::fromString(now_datetime, "yyyy-MM-dd hh-mm-ss"));
    this_page_data->setStartDateTime(QDateTime::fromString(start_datetime, "yyyy-MM-dd hh-mm-ss"));
    this_page_data->setEndDateTime(QDateTime::fromString(end_datetime, "yyyy-MM-dd hh-mm-ss"));
    this_page_data->setContent(ui->text_Edit->toPlainText());


    if(this->ifUpdate == false)
    {
        for(ScheduleData* schedule : ptr->ui->calendar->all_ScheduleList){
            if((schedule->startDateTime() <= this_page_data->startDateTime() && this_page_data->startDateTime() < schedule->endDateTime()) ||
               (schedule->startDateTime() < this_page_data->endDateTime() && this_page_data->endDateTime() <= schedule->endDateTime())){
                error = 3;
                qDebug()<<"mid";
                errorWidget->error_level = error;
                errorWidget->schedule->copySchedule(schedule);
                errorWidget->update_ErrorWidget();

                errorWidget->show();
                errorWidget->raise();


                qDebug()<<"3";

                break;
            }
        }

    }
    qDebug()<<"back";


    if(this_page_data->content() == ""){
        error = 1;
        errorWidget->error_level = error;
        errorWidget->update_ErrorWidget();

        errorWidget->show();
        errorWidget->raise();
        qDebug()<<"1";
    }
    else if(this_page_data->endDateTime() <= this_page_data->startDateTime()){
        error = 2;
        errorWidget->error_level = error;
        errorWidget->update_ErrorWidget();

        errorWidget->show();
        errorWidget->raise();
        qDebug()<<"2";
    }
    else if(error == 0){
        emit sendScheduleData(this_page_data);
        this->close();
    }
    QTimer::singleShot(200, this, SLOT(checkNeedUpdate()));
}

void new_page::checkNeedUpdate()
{
    emit checkNeedUpdateSignal();
}

void new_page::black_show()
{
    ui->widget->setStyleSheet("QWidget{background-color: rgb(46, 52, 54);}");
    ui->date_Edit->setStyleSheet("QDateEdit{color: rgb(186, 189, 182);}");
    ui->date_Label->setStyleSheet("QLabel{color: rgb(186, 189, 182);}");
    ui->time_start_Edit->setStyleSheet("QTimeEdit{color: rgb(186, 189, 182);}");
    ui->time_end_Edit->setStyleSheet("QTimeEdit{color: rgb(186, 189, 182);}");
    ui->time_Label->setStyleSheet("QLabel{color: rgb(186, 189, 182);}");
    ui->yes_Button->setStyleSheet("QPushButton{color: rgb(186, 189, 182);}");
    ui->no_Button->setStyleSheet("QPushButton{color: rgb(186, 189, 182);}");
    ui->text_Label->setStyleSheet("QLabel{color: rgb(186, 189, 182);}");
    ui->text_Edit->setStyleSheet("QTextEdit{color: rgb(186, 189, 182);}");
    ui->title->setStyleSheet("QLabel{color: rgb(186, 189, 182);}");
    ui->label->setStyleSheet("QLabel{color: rgb(186, 189, 182);}");
}

void new_page::light_show()
{

}

void new_page::mousePressEvent(QMouseEvent *event)
{
    m_isPressed = true;
    m_startMovePos = event->globalPos()-this->frameGeometry().topLeft();
}

void new_page::mouseMoveEvent(QMouseEvent *event)
{
    if(m_isPressed)
    {
        move(this->mapToGlobal(event->pos() - m_startMovePos));

    }
}

void new_page::mouseReleaseEvent(QMouseEvent *)
{
    m_isPressed = false;
}

void new_page::label_refresh()
{
    this->curDate = QDate::currentDate();
    this->selDate = this->ui->date_Edit->date();
    QString labelText = "";
    int yearDif = 0;
    int monthDif = 0;
    int weekDif = 0;
    int dayDif = 0;
    int dayofWeek = 0;

    yearDif = this->curDate.year() - this->selDate.year();
    monthDif = this->curDate.month() - this->selDate.month();
    weekDif = this->curDate.weekNumber() - this->selDate.weekNumber();
    dayDif = this->curDate.day() - this->selDate.day();
    dayofWeek = this->selDate.dayOfWeek();

    if(yearDif == 0 && monthDif == 0){
        switch (weekDif) {
            case 1: labelText.append("上周");
                switch (dayofWeek) {
                    case 1: labelText.append("一"); break;
                    case 2: labelText.append("二"); break;
                    case 3: labelText.append("三"); break;
                    case 4: labelText.append("四"); break;
                    case 5: labelText.append("五"); break;
                    case 6: labelText.append("六"); break;
                    case 7: labelText.append("日"); break;
                }
            break;
            case -1: labelText.append("下周");
                switch (dayofWeek) {
                    case 1: labelText.append("一"); break;
                    case 2: labelText.append("二"); break;
                    case 3: labelText.append("三"); break;
                    case 4: labelText.append("四"); break;
                    case 5: labelText.append("五"); break;
                    case 6: labelText.append("六"); break;
                    case 7: labelText.append("日"); break;
                }
            break;
            case 0:
                switch (dayDif) {
                    case 2: labelText.append("前天"); break;
                    case 1: labelText.append("昨天"); break;
                    case 0: labelText.append("今天"); break;
                    case -1: labelText.append("明天"); break;
                    case -2: labelText.append("后天"); break;
                default:
                    labelText.append("本周");
                    switch (dayofWeek) {
                        case 1: labelText.append("一"); break;
                        case 2: labelText.append("二"); break;
                        case 3: labelText.append("三"); break;
                        case 4: labelText.append("四"); break;
                        case 5: labelText.append("五"); break;
                        case 6: labelText.append("六"); break;
                        case 7: labelText.append("日"); break;
                    }

            } break;
        }
    }

    this->ui->label->setText(labelText);


}

void new_page::setUpdate()
{
    this->ifUpdate = true;
    QString updateString;
    updateString.append("修改日程");
    this->ui->title->setText(updateString);
}
