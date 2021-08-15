#include "monthScheduleCheck.h"
#include "ui_monthScheduleCheck.h"

#include <qdebug.h>

monthScheduleCheck::monthScheduleCheck(QWidget *parent) :
    QWidget(parent),
    m_isThemeChanged(false)
    , ui(new Ui::monthScheduleCheck)
    , m_newpage(Q_NULLPTR)
{
    ui->setupUi(this);

    QBitmap bmp(this->size());
    bmp.fill();
    QPainter p(&bmp);
    p.setPen(Qt::NoPen);
    p.setBrush(Qt::black);
    p.setRenderHint(QPainter::Antialiasing);
    p.drawRoundedRect(bmp.rect(),6,6);
    setMask(bmp);



    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setWindowFlags(Qt::FramelessWindowHint);
}

monthScheduleCheck::~monthScheduleCheck()
{
    delete ui;
}

void monthScheduleCheck::scheduleListInit(QDate date, QList<ScheduleData *> schedule)
{
    this->checkDate = date;
    for(ScheduleData* schedule : schedule){
        this->checkList.append(schedule);
    }
    this->dateString.append(QString::number(checkDate.month()));
    this->dateString.append("月");
    this->dateString.append(QString::number(checkDate.day()));
    this->dateString.append("日");

    this->ui->label->setText(this->dateString);

    color_clicked();
    checkUpdate(this->checkList);
}

void monthScheduleCheck::checkUpdate(QList<ScheduleData *> checkList)
{

    this->ui->listWidget->clear();

    this->ui->listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->ui->listWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    for(ScheduleData* schedule : checkList)
    {
        QListWidgetItem *item = new QListWidgetItem();
        monthScheduleBuddy *newBuddy = new monthScheduleBuddy();
        newBuddy->scheduleInit(schedule);
        connect(newBuddy, &monthScheduleBuddy::scheduleDeleteSignal, this, &monthScheduleCheck::checkDeleteScheduleSlots);
        connect(newBuddy, &monthScheduleBuddy::scheduleDeleteUpdateSignal, this, &monthScheduleCheck::checkUpdateScheduleSlots);
        connect(newBuddy, &monthScheduleBuddy::scheduleDeleteSignal, this, &monthScheduleCheck::checkInitScheduleSlots);

        connect(newBuddy, &monthScheduleBuddy::buddyNewScheduleSignal, this, &monthScheduleCheck::checkNewScheduleSlots);
        connect(newBuddy, &monthScheduleBuddy::buddyInitScheduleSignal, this, &monthScheduleCheck::checkInitScheduleSlots);
        item->setSizeHint(QSize(460, 150));
        this->ui->listWidget->addItem(item);
        this->ui->listWidget->setItemWidget(item, newBuddy);
    }
}

void monthScheduleCheck::color_clicked()
{
    if(this->m_isThemeChanged)
    {
        light_show();
    }else{

        black_show();
    }
}

void monthScheduleCheck::light_show()
{
    ui->widget->setStyleSheet("QWidget{background-color: rgba(255, 255, 255, 0.4);}");

    ui->label->setStyleSheet("background-color: rgba();\
                             font-size:20px;\
                                font-weight:400;\
                                color:rgba(40,40,40,1);\
                                line-height:34px;\
                                opacity:0.97;\
                                 ");
}

void monthScheduleCheck::black_show()
{
    ui->widget->setStyleSheet("QWidget{background-color: rgba(46, 52, 54, 0.4);}");
    ui->widget_2->setStyleSheet("QWidget{background-color: rgba(46, 52, 54, 0.4);}");
    ui->widget_3->setStyleSheet("QWidget{background-color: rgba(46, 52, 54, 0.4);}");
    ui->widget_4->setStyleSheet("QWidget{background-color: rgba(46, 52, 54, 0.4);}");
    ui->widget_5->setStyleSheet("QWidget{background-color: rgba(46, 52, 54, 0.4);}");
    ui->widget_6->setStyleSheet("QWidget{background-color: rgba(46, 52, 54, 0.4);}");
    ui->listWidget->setStyleSheet("QWidget{background-color: rgba(46, 52, 54, 0.4);border:none}");

    ui->label->setStyleSheet("background-color: rgba();\
                             font-size:20px;\
                                font-weight:400;\
                                color:rgba(255,255,255,1);\
                                line-height:34px;\
                                opacity:0.97;\
                                 ");
}

void monthScheduleCheck::on_exit_Button_clicked()
{
    this->close();
}

void monthScheduleCheck::on_add_Button_clicked()
{
    this->m_newpage = new new_page();
    m_newpage->checkNewInit(this->checkDate);
    m_newpage->setWindowModality(Qt::ApplicationModal);
    connect(this->m_newpage, &new_page::sendScheduleData, this, &monthScheduleCheck::checkNewScheduleSlots);
    connect(this->m_newpage, &new_page::sendScheduleData, this, &monthScheduleCheck::checkInitScheduleSlots);
    connect(this->m_newpage, &new_page::checkNeedUpdateSignal,this, &monthScheduleCheck::checkUpdateScheduleSlots);
    m_newpage->show();
}

void monthScheduleCheck::checkNewScheduleSlots(ScheduleData* schedule)
{
    emit checkNewScheduleSignal(schedule);
}

void monthScheduleCheck::checkInitScheduleSlots(ScheduleData* schedule)
{
    emit checkInitScheduleSignal(schedule);
}

void monthScheduleCheck::checkDeleteScheduleSlots(ScheduleData* schedule)
{
    emit checkDeleteScheduleSignal(schedule);
}

void monthScheduleCheck::checkUpdateScheduleSlots()
{
    Q_UNUSED(checkList);
    emit checkUpdateScheduleSignal();
}
void monthScheduleCheck::mousePressEvent(QMouseEvent *event)
{
    m_isPressed = true;
    m_startMovePos = event->globalPos()-this->frameGeometry().topLeft();
}

void monthScheduleCheck::mouseMoveEvent(QMouseEvent *event)
{
    if(m_isPressed)
    {
        move(this->mapToGlobal(event->pos() - m_startMovePos));

    }
}

void monthScheduleCheck::mouseReleaseEvent(QMouseEvent *)
{
    m_isPressed = false;
}
