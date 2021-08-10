#ifndef DBMANAGER_H
#define DBMANAGER_H

#include "scheduledata.h"
#include <QObject>
#include <QtSql/QSqlDatabase>

class DBManager : public QObject
{
Q_OBJECT

public:

    DBManager(QObject *parent = Q_NULLPTR);

private:

    void open(const QString& path, bool doCreate = false);
    void createTables();
    int  getLastRowID();
    bool forceLastRowIndexValue(const int indexValue);
    ScheduleData* getSchedulebyDay(QDateTime selectDateTime);
    bool isScheduleExist(ScheduleData* schedule);
    QList<ScheduleData *> getAllSchedules();
    bool addSchedule(ScheduleData* schedule);
    bool removeSchedule(ScheduleData* schedule);
    bool permanantlyRemoveAllSchedules();
    bool updateSchedule(ScheduleData* schedule);
    bool migrateSchedule(ScheduleData* schedule);
    bool migrateTrash(ScheduleData* schedule);

signals:

    void notesReceived(QList<ScheduleData*> scheduleList, int scheduleCounter);

public slots:

    void onSchedulesListRequested();
    void onOpenDBManagerRequested(QString path, bool doCreate);
    void onCreateUpdateRequested(ScheduleData* schedule);
    void onDeleteScheduleRequested(ScheduleData* schedule);
    void onImportSchedulesRequested(QList<ScheduleData *> scheduleList);
    void onRestoreSchedulesRequested(QList<ScheduleData *> scheduleList);
    void onExportSchedulesRequested(QString fileName);
    void onMigrateSchedulesRequested(QList<ScheduleData *> scheduleList);
    void onMigrateTrashRequested(QList<ScheduleData *> scheduleList);
    void onForceLastRowIndexValueRequested(int index);
};

#endif // DBMANAGER_H
