#ifndef MAINCALENDAR_H
#define MAINCALENDAR_H

#include <QObject>
#include <QWidget>
#include <QDate>
#include <QPainter>
#include <QMouseEvent>

enum page_state {year_state, month_state, day_state};

class MainCalendar : public QWidget
{

    Q_OBJECT

public:
    explicit MainCalendar(QWidget *parent = nullptr);
    ~MainCalendar();

    page_state STATE = day_state;   //日历 年 月 日选择

    struct DateItem {               //日期管理表
        int year;
        int month;
        int day;

        DateItem()
        {
            year = -1;
            month = -1;
            day = -1;
        }
    };

    void updateCalendar(const QDate &selectDate);   //更新日历表

protected:
    void paintEvent(QPaintEvent *);


private:
    QColor bgColor;
    QColor textColor;
    QColor shadowColor;
    QColor selectColor;

    QDate selectDate = QDate::currentDate();    //选中的日期
    QDate today = QDate::currentDate();         //今天的日期

    DateItem dateItem[6][7];    //日期表
    int yearSelect[3][3];       //年份表
    int monthSelect[3][4];      //月份表

public slots:
    QDate readSelectDate();     //返回选中日期
    QString readPageState();    //返回日历状态
    void setSelectDate(const QDate &selectDate);    //设置选中日期
    void setPageState();                            //设置日历状态
    void mousePressEvent(QMouseEvent *event);

signals:


};

#endif // MAINCALENDAR_H
