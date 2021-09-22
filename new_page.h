#ifndef NEW_PAGE_H
#define NEW_PAGE_H

#include <QWidget>
#include <QDateTime>
#include <QDate>
#include <QTime>
#include <QTimer>
#include <QMouseEvent>
#include <QTimer>

#include "scheduledata.h"
#include "adderrorwidget.h"

namespace Ui {
class new_page;
}

class new_page : public QWidget
{
    Q_OBJECT

    bool page_theme = false;

public:
    explicit new_page(QWidget *parent = nullptr);
    ~new_page();

    void checkNewInit(QDate date);

    void buddyUpdateInit(ScheduleData* schedule);

    void checkUpdateInit(ScheduleData* updateSchedule);

private:

    Ui::new_page *ui;

    bool m_isPressed = false;
    QPoint m_startMovePos;
    QTimer* mTimerRefresh;
    ScheduleData *this_page_data;
    QDate curDate;
    QDate selDate;

    void black_show();
    void light_show();

    void mousePressEvent(QMouseEvent *event);

    void mouseMoveEvent(QMouseEvent *event);

    void mouseReleaseEvent(QMouseEvent *);
                                    //设置拖拽





private slots:

    void on_no_Button_clicked();
    void onScheduleDataCreated();
    void checkNeedUpdate();

    void label_refresh();

signals:

    void sendScheduleData(ScheduleData* schedule);
    void checkNeedUpdateSignal();

};

#endif // NEW_PAGE_H
