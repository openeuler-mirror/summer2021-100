#include "scheduleExitWindow.h"
#include "ui_scheduleExitWindow.h"
#include "widget.h"
#include "ui_widget.h"

scheduleExitWindow::scheduleExitWindow(Widget* page, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::scheduleExitWindow)
    , tray(Q_NULLPTR)
{
    ui->setupUi(this);
    pNotebook = page;
    tray = customSystemTrayIcon::instance(page);
    tray->setTray("日程管理", ":/myImage/calendar.svg", "日程管理已隐藏");
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
    //connect(pNotebook->ui->change_Button,SIGNAL(clicked()),this,SLOT(color_clicked()));
    color_clicked();
}
scheduleExitWindow::~scheduleExitWindow()
{
    delete ui;
}

void scheduleExitWindow::on_pushButton_2_clicked()
{
    this->close();
}

void scheduleExitWindow::on_pushButton_clicked()
{
    //qDebug() << "当前文件 :" << __FILE__ << "当前函数 :" << __FUNCTION__ << "当前行号 :" << __LINE__;
    this->close();

    if(ui->checkBox->isChecked()==true)
    {
        /**
         *  这里是支持任务栏托盘
         **/
        tray->showTray();
        pNotebook->hide();
    }
    else
    {
        pNotebook->close();
    }
}

void scheduleExitWindow::color_clicked()
{
    if(pNotebook->m_isThemeChanged)
    {
        light_show();
    }else{

        black_show();
    }
}

void scheduleExitWindow::light_show()
{
    ui->widget->setStyleSheet("QWidget{background-color: rgba(255, 255, 255, 0.4);}");

    ui->label->setStyleSheet("background-color: rgba();\
                             font-size:20px;\
                                font-weight:400;\
                                color:rgba(40,40,40,1);\
                                line-height:34px;\
                                opacity:0.97;\
                                 ");
     ui->checkBox->setStyleSheet("background-color: rgba();\
                                font-size:14px;\
                                font-weight:400;\
                                color:rgba(102,102,102,1);\
                                line-height:40px;\
                                opacity:0.91;");
     ui->pushButton->setStyleSheet(QString::fromUtf8("color:rgba(102,102,102,1);\n"
                                                 "line-height:24px;"));
     ui->pushButton_2->setStyleSheet(QString::fromUtf8("color:rgba(102,102,102,1);\n"
                                                 "line-height:24px;"));
}

void scheduleExitWindow::black_show()
{
    ui->widget->setStyleSheet("QWidget{background-color: rgba(46, 52, 54, 0.4);}");

    ui->label->setStyleSheet("background-color: rgba();\
                             font-size:20px;\
                                font-weight:400;\
                                color:rgba(255,255,255,1);\
                                line-height:34px;\
                                opacity:0.97;\
                                 ");
     ui->checkBox->setStyleSheet("background-color: rgba();\
                                 font-size:14px;\
                                 font-weight:400;\
                                 color:rgba(255,255,255,1);\
                                 line-height:40px;\
                                 opacity:0.91;");
     ui->pushButton->setStyleSheet(QString::fromUtf8("color:rgba(255,255,255,1);\n"
                                                      "line-height:24px;"));
     ui->pushButton_2->setStyleSheet(QString::fromUtf8("color:rgba(255,255,255,1);\n"
                                                      "line-height:24px;"));
}
