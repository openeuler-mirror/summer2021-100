#ifndef LINESCHEDULELABEL_H
#define LINESCHEDULELABEL_H

#include <QObject>
#include <QWidget>
#include <QDate>
#include <QString>

namespace Ui {
class lineScheduleLabel;
}

class lineScheduleLabel : public QWidget
{
    Q_OBJECT
public:
    explicit lineScheduleLabel(QWidget *parent = nullptr);
    ~lineScheduleLabel();

    void initLabel(QDate date);

private:
    Ui::lineScheduleLabel *ui;

private slots:

signals:

};

#endif // LINESCHEDULELABEL_H
