#ifndef NEW_PAGE_H
#define NEW_PAGE_H

#include <QWidget>
#include <QDateTime>
#include <QDate>
#include <QTime>

#include "scheduledata.h"

namespace Ui {
class new_page;
}

class new_page : public QWidget
{
    Q_OBJECT

public:
    explicit new_page(QWidget *parent = nullptr);
    ~new_page();

private:

    Ui::new_page *ui;

    ScheduleData *this_page_data;



private slots:

    void on_no_Button_clicked();
    void onScheduleDataCreated();

signals:

    void sendScheduleData(ScheduleData* schedule);

};

#endif // NEW_PAGE_H
