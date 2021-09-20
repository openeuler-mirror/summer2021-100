#ifndef NEW_PAGE_H
#define NEW_PAGE_H

#include <QWidget>
#include <QDateTime>
#include <QDate>
#include <QTime>
#include <QTimer>
#include <QMouseEvent>

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

    bool m_isPressed = false;
    QPoint m_startMovePos;

    Ui::new_page *ui;

    ScheduleData *this_page_data;

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

signals:

    void sendScheduleData(ScheduleData* schedule);
    void checkNeedUpdateSignal();

};

#endif // NEW_PAGE_H
