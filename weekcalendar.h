#ifndef WEEKCALENDAR_H
#define WEEKCALENDAR_H

#include <QObject>
#include <QWidget>

class weekCalendar : public QWidget
{
    Q_OBJECT
public:
    explicit weekCalendar(QWidget *parent = nullptr);
    ~weekCalendar();
};

#endif // WEEKCALENDAR_H
