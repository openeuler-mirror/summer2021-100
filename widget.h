#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSettings>
#include <QThread>
#include <QTimer>
#include <QTime>
#include <QDate>

#include "mainCalendar.h"
#include "new_page.h"
#include "dbmanager.h"

namespace Ui {
    class schedule;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    Ui::schedule *ui;               //主UI








private:
    QString dateString;             //显示当前日期的字符串
    QDate* date;                    //当前的日期
    QTime* time;                    //当前的时间
    QTimer* mTimerRefresh;          //画面刷新定时器 20ms一刷新
    new_page* np;                   //新建日程窗口
    QSettings* m_settingsDatabase;  //配置文件
    DBManager* m_dbManager;         //数据库
    QThread* m_dbThread;            //数据库线程

    void kyScheduleInit();          //加载界面组件
    void kyScheduleConn();          //绑定槽函数
    void updateTimeButton();        //刷新时间显示
    void updateYearButton();        //刷新日期显示
    void createNewSchedule();       //新建日程
    void deleteSchedule(const QModelIndex& noteIndex, bool isFromUser=true);
                                    //删除日程
    void setupDatabases();          //配置数据库
    void initializeSettingsDatabase();
                                    //初始化配置文件
    void saveNoteToDB(const QModelIndex& noteIndex);
                                    //保存到数据库
    ScheduleData* generateNote(const int scheduleID);
                                    //新建日程时初始化


private slots:
    void widget_refresh();          //定时器溢出处理 画面刷新 20ms
    void on_yearButton_clicked();   //点击年份显示按钮
    void on_newButton_clicked();    //点击新建按钮

signals:


};

#endif // WIDGET_H
