#ifndef MONTHSCHEDULECHECK_H
#define MONTHSCHEDULECHECK_H

#include <QWidget>
#include <QListWidget>
#include <QListWidgetItem>
#include <QDate>
#include <QPainter>
#include <QBitmap>
#include <QMouseEvent>
#include <QTimer>

#include "new_page.h"
#include "scheduledata.h"
#include "monthscheduleBuddy.h"


namespace Ui {
class monthScheduleCheck;
}

class monthScheduleCheck : public QWidget
{
    Q_OBJECT

public:
    explicit monthScheduleCheck(QWidget *parent=nullptr);
    ~monthScheduleCheck();

    void scheduleListInit(QDate date, QList<ScheduleData *> schedule);
    void checkUpdate(QList<ScheduleData *> checkList);


    bool m_isThemeChanged;



private slots:

    void color_clicked();

    void on_exit_Button_clicked();
    void on_add_Button_clicked();



    void checkNewScheduleSlots(ScheduleData* schedule);
    void checkInitScheduleSlots(ScheduleData* schedule);
    void checkDeleteScheduleSlots(ScheduleData* schedule);
    void checkUpdateScheduleSlots();



private:
    Ui::monthScheduleCheck *ui;

    QDate checkDate;
    QString dateString;
    QList<ScheduleData *> checkList;

    bool m_isPressed = false;
    QPoint m_startMovePos;

    new_page* m_newpage;

    void light_show();
    void black_show();

    void mousePressEvent(QMouseEvent *event);

    void mouseMoveEvent(QMouseEvent *event);

    void mouseReleaseEvent(QMouseEvent *);
                                    //设置拖拽

signals:

    void checkNewScheduleSignal(ScheduleData* schedule);
    void checkInitScheduleSignal(ScheduleData* schedule);
    void checkDeleteScheduleSignal(ScheduleData* schedule);
    void checkUpdateScheduleSignal();
};

#endif // MONTHSCHEDULECHECK_H
