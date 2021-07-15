#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <QWidget>
#include "maincalendar.h"

namespace Ui {
    class schedule;
}

class schedule : public QWidget
{
    Q_OBJECT

public:
    explicit schedule(QWidget *parent = 0);
    ~schedule();

private slots:

    void on_pushButton_clicked();

private:
    Ui::schedule *ui;
};

#endif // SCHEDULE_H
