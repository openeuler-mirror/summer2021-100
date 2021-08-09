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
    ScheduleData* getNote(QString id);
    bool isNoteExist(ScheduleData* schedule);
    QList<ScheduleData *> getAllNotes();
    bool addNote(ScheduleData* schedule);
    bool removeNote(ScheduleData* schedule);
    bool permanantlyRemoveAllNotes();
    bool updateNote(ScheduleData* schedule);
    bool migrateNote(ScheduleData* schedule);
    bool migrateTrash(ScheduleData* schedule);

signals:

    void notesReceived(QList<ScheduleData*> scheduleList, int scheduleCounter);

public slots:

    void onNotesListRequested();
    void onOpenDBManagerRequested(QString path, bool doCreate);
    void onCreateUpdateRequested(ScheduleData* schedule);
    void onDeleteNoteRequested(ScheduleData* schedule);
    void onImportNotesRequested(QList<ScheduleData *> scheduleList);
    void onRestoreNotesRequested(QList<ScheduleData *> scheduleList);
    void onExportNotesRequested(QString fileName);
    void onMigrateNotesRequested(QList<ScheduleData *> scheduleList);
    void onMigrateTrashRequested(QList<ScheduleData *> scheduleList);
    void onForceLastRowIndexValueRequested(int index);
};

#endif // DBMANAGER_H
