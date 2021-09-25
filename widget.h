#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QApplication>
#include <QDesktopWidget>
#include <QSettings>
#include <QThread>
#include <QTimer>
#include <QTime>
#include <QDate>
#include <QFileInfo>
#include <QDir>
#include <QProgressDialog>
#include <QFuture>
#include <QFutureWatcher>
#include <QtConcurrent>
#include <QMouseEvent>

#include "mainCalendar.h"
#include "new_page.h"
#include "dbmanager.h"
#include "scheduleExitWindow.h"

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
    bool m_isThemeChanged;          //主题色改变








private:
    QString dateString;             //显示当前日期的字符串
    QDate* date;                    //当前的日期
    QTime* time;                    //当前的时间
    QTimer* mTimerRefresh;          //画面刷新定时器 20ms一刷新
    new_page* np;                   //新建日程窗口
    QSettings* m_settingsDatabase;  //配置文件
    DBManager* m_dbManager;         //数据库
    QThread* m_dbThread;            //数据库线程
    scheduleExitWindow* m_scheduleExitWindow=nullptr;
                                    //退出弹窗

    int m_scheduleCounter;          //日程总数
    int m_trashCounter;             //垃圾桶总数

    bool m_isPressed = false;
    QPoint m_startMovePos;
                                    //设置拖拽参数

    void kyScheduleInit();          //加载界面组件
    void kyScheduleConn();          //绑定槽函数
    void updateTimeButton();        //刷新时间显示
    void updateYearButton();        //刷新日期显示
    void updateWeekButton();
    void createNewSchedule();       //新建日程
    void deleteSchedule(ScheduleData* schedule);
                                    //删除日程
    void setupDatabases();          //配置数据库
    void initializeSettingsDatabase();
                                    //初始化配置文件
    void saveNoteToDB(const QModelIndex& noteIndex);
                                    //保存到数据库
    ScheduleData* generateNote(const int scheduleID);
                                    //新建日程时初始化


    void checkMigration();          //迁移sync 还没整好
    void migrateSchedule(QString notePath);
                                    //便签数据迁移 还没整好
    void black_show();              //显示黑色模式

    void mousePressEvent(QMouseEvent *event);

    void mouseMoveEvent(QMouseEvent *event);

    void mouseReleaseEvent(QMouseEvent *);
                                    //设置拖拽





private slots:
    void InitData();                //初始化/迁移数据
    void widget_refresh();          //定时器溢出处理 画面刷新 20ms
    void on_yearButton_clicked();   //点击年份显示按钮
    void on_newButton_clicked();    //点击新建按钮
    void on_month_Button_clicked();
    void on_week_Button_clicked();
    void on_schedule_Button_clicked();
    void on_weekButton_clicked();
    void on_nextButton_clicked();
    void on_preButton_clicked();
    void create_update_slots(ScheduleData *schedule);
                                    //增改槽函数
    void loadSchedules(QList<ScheduleData *> scheduleList);
                                    //加载日程列表
    void miniSlot();                //最小化按钮槽函数
    void exitSlot();                //退出槽函数


signals:

    void requestSchedulesList();                                            //加载列表请求信号
    void requestOpenDBManager(QString path, bool doCreate);                 //打开数据库信号
    void requestCreateUpdateSchedule(ScheduleData* note);                   //数据库更新信号
    void requestDeleteSchedule(ScheduleData* note);                         //数据库同步删除信号
    void requestRestoreSchedules(QList<ScheduleData *> noteList);           //重加载信号
    void requestImportSchedules(QList<ScheduleData *> noteList);            //导入信号
    void requestExportSchedules(QString fileName);                          //导出信号
    void requestMigrateSchedules(QList<ScheduleData *> noteList);           //迁移信号
    void requestMigrateTrash(QList<ScheduleData *> noteList);               //迁移废纸篓信号
    void requestForceLastRowIndexValue(int index);                          //请求返回受结果的SQL语句影响的行数信号
};

#endif // WIDGET_H
