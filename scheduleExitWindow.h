#ifndef SCHEDULEEXITWINDOW_H
#define SCHEDULEEXITWINDOW_H


#include <QDialog>
#include <QPainter>
#include <QBitmap>

#include "customSystemTrayicon.h"

class Widget;
namespace Ui {
class scheduleExitWindow;
}

class scheduleExitWindow :public QDialog
{
Q_OBJECT

public:
    explicit scheduleExitWindow(Widget* page,QWidget *parent=nullptr);
    ~scheduleExitWindow();


private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void color_clicked();

private:
    Ui::scheduleExitWindow *ui;

    customSystemTrayIcon* tray;

    void light_show();
    void black_show();

    Widget* pNotebook;
};

#endif // SCHEDULEEXITWINDOW_H
