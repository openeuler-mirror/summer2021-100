#ifndef MAINCALENDAR_H
#define MAINCALENDAR_H

#include <QObject>
#include <QWidget>
#include <QDate>
#include <QPainter>
#include <QMouseEvent>


class MainCalendar : public QWidget
{

    Q_OBJECT
    Q_PROPERTY(QColor bgColor READ getBgColor WRITE setBgColor)
    Q_PROPERTY(QColor textColor READ getTextColor WRITE setTextColor)
    Q_PROPERTY(QColor shadowColor READ getShadowColor WRITE setShadowColor)
    Q_PROPERTY(QColor selectColor READ getSelectColor WRITE setSelectColor)

public:

    enum page_state {year_state, month_state, day_state};
    Q_ENUM(page_state);
    page_state STATE = day_state;

    struct DateItem {
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

    QColor getBgColor()         const;
    QColor getTextColor()       const;
    QColor getShadowColor()     const;
    QColor getSelectColor()     const;

    void updateCalendar(const QDate &selectDate);
    explicit MainCalendar(QWidget *parent = nullptr);
    ~MainCalendar();

protected:
    void paintEvent(QPaintEvent *);


private:
    QColor bgColor;
    QColor textColor;
    QColor shadowColor;
    QColor selectColor;

    QDate selectDate = QDate::currentDate();
    QDate today = QDate::currentDate();

    DateItem dateItem[6][7];
    int yearSelect[3][3];
    int monthSelect[3][4];

public Q_SLOTS:
    void setBgColor(const QColor &bgColor);
    void setTextColor(const QColor &textColor);
    void setShadowColor(const QColor &shadowColor);
    void setSelectColor(const QColor &selectColor);
    void setSelectDate(const QDate &selectDate);
    void setPageState();

    QDate readSelectDate();
    QString readPageState();
    void mousePressEvent(QMouseEvent *event);
};

#endif // MAINCALENDAR_H
