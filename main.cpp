#include "schedule.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    schedule w;
    w.show();
    return a.exec();
}
