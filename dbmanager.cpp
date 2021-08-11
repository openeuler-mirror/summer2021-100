#include "dbmanager.h"

#include <QtSql/QSqlQuery>
#include <QTimeZone>
#include <QDateTime>
#include <QDebug>
#include <QSqlError>
#include <QtConcurrent>

DBManager::DBManager(QObject *parent)
    : QObject(parent)
{
    qRegisterMetaType<QList<ScheduleData*> >("QList<ScheduleData*>");
}

void DBManager::open(const QString &path, bool doCreate)
{
    QSqlDatabase m_db;
    m_db = QSqlDatabase::addDatabase("QSQLITE","schedule");

    m_db.setDatabaseName(path);
    if(!m_db.open()){
        qDebug() << "Error: connection with database fail";
    }else{
        qDebug() << "Database: connection ok";
    }

    if(doCreate)
        createTables();
}

void DBManager::createTables()
{
    QSqlQuery query(QSqlDatabase::database("schedule"));
    QString active = "CREATE TABLE active_schedules ("
                     "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                     "creation_date INTEGER NOT NULL DEFAULT (0),"
                     "modification_date INTEGER NOT NULL DEFAULT (0),"
                     "deletion_date INTEGER NOT NULL DEFAULT (0),"
                     "start_datetime INTEGET NOT NULL DEFAULT (0),"
                     "end_datetime INTEGET NOT NULL DEFAULT (0),"
                     "content TEXT, "
                     "schedule_color INTEGER DEFAULT (0),"
                     "md_content TEXT);";

    query.exec(active);
    QString active_index = "CREATE UNIQUE INDEX active_index on active_schedules (id ASC);";
    query.exec(active_index);

    QString deleted = "CREATE TABLE deleted_schedules ("
                      "id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
                      "creation_date INTEGER NOT NULL DEFAULT (0),"
                      "modification_date INTEGER NOT NULL DEFAULT (0),"
                      "deletion_date INTEGER NOT NULL DEFAULT (0),"
                      "start_datetime INTEGET NOT NULL DEFAULT (0),"
                      "end_datetime INTEGET NOT NULL DEFAULT (0),"
                      "content TEXT,"
                      "schedule_color INTEGER DEFAULT (0),"
                      "md_content TEXT);";
    query.exec(deleted);
}

int DBManager::getLastRowID()
{
    QSqlQuery query(QSqlDatabase::database("schedule"));
    query.exec("SELECT seq from SQLITE_SEQUENCE WHERE name='active_schedules';");
    query.next();
    return query.value(0).toInt();
}

bool DBManager::forceLastRowIndexValue(const int indexValue)
{
    QSqlQuery query(QSqlDatabase::database("schedule"));
    QString queryStr = QStringLiteral("UPDATE SQLITE_SEQUENCE "
                                      "SET seq=%1 "
                                      "WHERE name='active_schedules';").arg(indexValue);
    query.exec(queryStr);
    return query.numRowsAffected() == 1;
}

ScheduleData* DBManager::getSchedulebyDay(QDateTime selectDateTime)
{
    QSqlQuery query(QSqlDatabase::database("schedule"));

    int parsedId = 1;
    QString queryStr = QStringLiteral("SELECT * FROM active_schedules WHERE id = %1 LIMIT 1").arg(parsedId);
    query.exec(queryStr);
    //检索结果中的第一条记录（如果有），并将查询放在检索到的记录上
    if (query.first()) {
        ScheduleData* schedule = new ScheduleData(this->parent() == Q_NULLPTR ? Q_NULLPTR : this);
        int id =  query.value(0).toInt();
        qint64 epochDateTimeCreation = query.value(1).toLongLong();
        QDateTime dateTimeCreation = QDateTime::fromMSecsSinceEpoch(epochDateTimeCreation);
        qint64 epochDateTimeModification= query.value(2).toLongLong();
        QDateTime dateTimeModification = QDateTime::fromMSecsSinceEpoch(epochDateTimeModification);
        QString content = query.value(4).toString();
        qint64 epochDateTimeStart = query.value(5).toLongLong();
        QDateTime dateTimeStart = QDateTime::fromMSecsSinceEpoch(epochDateTimeStart);
        qint64 epochDateTimeEnd = query.value(6).toLongLong();
        QDateTime dateTimeEnd = QDateTime::fromMSecsSinceEpoch(epochDateTimeEnd);
        int schedulecolor = query.value(7).toInt();
        QString mdContent = query.value(8).toString();

        schedule->setId(id);
        schedule->setCreationDateTime(dateTimeCreation);
        schedule->setLastModificationDateTime(dateTimeModification);
        schedule->setContent(content);
        schedule->setStartDateTime(dateTimeStart);
        schedule->setEndDateTime(dateTimeEnd);
        schedule->setScheduleColor(schedulecolor);
        schedule->setMdContent(mdContent);

        return schedule;
    }
    return Q_NULLPTR;
}

bool DBManager::isScheduleExist(ScheduleData* schedule)
{
    QSqlQuery query(QSqlDatabase::database("schedule"));

    int id = schedule->id();
    QString queryStr = QStringLiteral("SELECT EXISTS(SELECT 1 FROM active_schedules WHERE id = %1 LIMIT 1 )")
            .arg(id);
    query.exec(queryStr);
    query.next();

    return query.value(0).toInt() == 1;
}

QList<ScheduleData *> DBManager::getAllSchedules()
{
    QList<ScheduleData *> scheduleList;

    QSqlQuery query(QSqlDatabase::database("schedule"));
    query.prepare("SELECT * FROM active_schedules");
    bool status = query.exec();
    if(status){
        while(query.next()){
            ScheduleData* schedule = new ScheduleData(this);
            int id =  query.value(0).toInt();
            qint64 epochDateTimeCreation = query.value(1).toLongLong();

            QDateTime dateTimeCreation = QDateTime::fromMSecsSinceEpoch(epochDateTimeCreation);
            qint64 epochDateTimeModification= query.value(2).toLongLong();
            QDateTime dateTimeModification = QDateTime::fromMSecsSinceEpoch(epochDateTimeModification);
            QString content = query.value(4).toString();
            qint64 epochDateTimeStart = query.value(5).toLongLong();
            QDateTime dateTimeStart = QDateTime::fromMSecsSinceEpoch(epochDateTimeStart);
            qint64 epochDateTimeEnd = query.value(6).toLongLong();
            QDateTime dateTimeEnd = QDateTime::fromMSecsSinceEpoch(epochDateTimeEnd);
            int schedulecolor = query.value(7).toInt();
            QString mdContent = query.value(8).toString();

            schedule->setId(id);
            schedule->setCreationDateTime(dateTimeCreation);
            schedule->setLastModificationDateTime(dateTimeModification);
            schedule->setContent(content);
            schedule->setStartDateTime(dateTimeStart);
            schedule->setEndDateTime(dateTimeEnd);
            schedule->setScheduleColor(schedulecolor);
            schedule->setMdContent(mdContent);

            scheduleList.push_back(schedule);
        }
    }

    return scheduleList;
}

bool DBManager::addSchedule(ScheduleData* schedule)
{
    QSqlQuery query(QSqlDatabase::database("schedule"));
    QString emptyStr;

    qint64 epochTimeDateCreated = schedule->creationDateTime()
            .toMSecsSinceEpoch();

    QString content = schedule->content()
                            .replace("'","''")
                            .replace(QChar('\x0'), emptyStr);

    qint64 epochTimeDateStart = schedule->startDateTime()
            .toMSecsSinceEpoch();

    qint64 epochTimeDateEnd = schedule->endDateTime()
            .toMSecsSinceEpoch();

    QString mdContent = schedule->mdContent()
                              .replace("'","''")
                              .replace(QChar('\x0'), emptyStr);

    int schedulecolor = schedule->scheduleColor();

    qint64 epochTimeDateLastModified = schedule->lastModificationdateTime().isNull() ? epochTimeDateCreated
                                                                                 : schedule->lastModificationdateTime().toMSecsSinceEpoch();

    QString queryStr = QString("INSERT INTO active_schedules "
                               "(creation_date, modification_date, deletion_date, content, start_datetime, end_datetime, schedule_color, md_content) "
                               "VALUES (%1, %2, -1, '%3', %4, %5, '%6', '%7');")
                               .arg(epochTimeDateCreated)
                               .arg(epochTimeDateLastModified)
                               .arg(content)
                               .arg(epochTimeDateStart)
                               .arg(epochTimeDateEnd)
                               .arg(schedulecolor)
                               .arg(mdContent);

    query.exec(queryStr);
    //Returns the number of rows affected by the result's SQL statement
    return (query.numRowsAffected() == 1);
}

bool DBManager::removeSchedule(ScheduleData *schedule)
{
    QSqlQuery query(QSqlDatabase::database("schedule"));
    QString emptyStr;

    int id = schedule->id();
    QString queryStr = QStringLiteral("DELETE FROM active_schedules "
                                      "WHERE id=%1").arg(id);
    query.exec(queryStr);
    bool removed = (query.numRowsAffected() == 1);

    qint64 epochTimeDateCreated = schedule->creationDateTime().toMSecsSinceEpoch();
    qint64 epochTimeDateModified = schedule->lastModificationdateTime().toMSecsSinceEpoch();
    qint64 epochTimeDateDeleted = schedule->deletionDateTime().toMSecsSinceEpoch();
    QString content = schedule->content()
                            .replace("'","''")
                            .replace(QChar('\x0'), emptyStr);

    qint64 epochTimeDateStart = schedule->startDateTime().toMSecsSinceEpoch();
    qint64 epochTImeDateEnd = schedule->endDateTime().toMSecsSinceEpoch();

    QString mdContent = schedule->mdContent()
                            .replace("'","''")
                            .replace(QChar('\x0'), emptyStr);

    int schedulecolor = schedule->scheduleColor();

    queryStr = QString("INSERT INTO deleted_schedules "
                       "VALUES (%1, %2, %3, %4, '%5', %6, %7, '%8', '%9');")
                       .arg(id)
                       .arg(epochTimeDateCreated)
                       .arg(epochTimeDateModified)
                       .arg(epochTimeDateDeleted)
                       .arg(content)
                       .arg(epochTimeDateStart)
                       .arg(epochTImeDateEnd)
                       .arg(schedulecolor)
                       .arg(mdContent);

    query.exec(queryStr);
    bool addedToTrashDB = (query.numRowsAffected() == 1);

    return (removed && addedToTrashDB);
}

bool DBManager::permanantlyRemoveAllSchedules()
{
    QSqlQuery query(QSqlDatabase::database("schedule"));
    return query.exec(QString("DELETE FROM active_schedules"));
}

bool DBManager::updateSchedule(ScheduleData *schedule)
{
    QSqlQuery query(QSqlDatabase::database("note"));
    QString emptyStr;

    int id = schedule->id();
    qDebug() << "updateNote  note id = " <<id;
    int schedulecolor = schedule->scheduleColor();

    qint64 epochTimeDateModified = schedule->lastModificationdateTime().toMSecsSinceEpoch();
    QString content = schedule->content().replace(QChar('\x0'), emptyStr);
    qint64 epochTimeDateStart = schedule->startDateTime().toMSecsSinceEpoch();
    qint64 epochTimeDateEnd = schedule->endDateTime().toMSecsSinceEpoch();
    QString mdContent = schedule->mdContent().replace(QChar('\x0'), emptyStr);

    query.prepare(QStringLiteral("UPDATE active_schedules SET md_content = :mdContent, schedule_color = :color,"
                                 "modification_date = :date, content = :content, start_datetime = :start_date, end_datetime = :end_date WHERE id = :id"));
    query.bindValue(QStringLiteral(":mdContent"), mdContent);
    query.bindValue(QStringLiteral(":color"), schedulecolor);
    query.bindValue(QStringLiteral(":date"), epochTimeDateModified);
    query.bindValue(QStringLiteral(":content"), content);
    query.bindValue(QStringLiteral(":start_date"), epochTimeDateStart);
    query.bindValue(QStringLiteral(":end_date"), epochTimeDateEnd);
    query.bindValue(QStringLiteral(":id"), id);


    if (!query.exec()) {
        qWarning () << __func__ << ": " << query.lastError();
    }

    return (query.numRowsAffected() == 1);
}

bool DBManager::migrateSchedule(ScheduleData* schedule)
{
    QSqlQuery query(QSqlDatabase::database("schedule"));

    QString emptyStr;

    int id = schedule->id();
    int notecolor = schedule->scheduleColor();
    qint64 epochTimeDateCreated = schedule->creationDateTime().toMSecsSinceEpoch();
    qint64 epochTimeDateModified = schedule->lastModificationdateTime().toMSecsSinceEpoch();
    QString content = schedule->content()
                            .replace("'","''")
                            .replace(QChar('\x0'), emptyStr);
    qint64 epochTimeDateStart = schedule->startDateTime().toMSecsSinceEpoch();
    qint64 epochTimeDateEnd = schedule->endDateTime().toMSecsSinceEpoch();
    QString mdContent = schedule->mdContent()
                            .replace("'","''")
                            .replace(QChar('\x0'), emptyStr);

    QString queryStr = QString("INSERT INTO active_schedules "
                               "VALUES (%1, %2, %3, -1, '%4', %5, %6, '%7', '%8');")
                               .arg(id)
                               .arg(epochTimeDateCreated)
                               .arg(epochTimeDateModified)
                               .arg(content)
                               .arg(epochTimeDateStart)
                               .arg(epochTimeDateEnd)
                               .arg(notecolor)
                               .arg(mdContent);

    query.exec(queryStr);
    return (query.numRowsAffected() == 1);
}

bool DBManager::migrateTrash(ScheduleData* schedule)
{
    QSqlQuery query(QSqlDatabase::database("schedule"));
    QString emptyStr;

    int id = schedule->id();
    int notecolor = schedule->scheduleColor();
    qint64 epochTimeDateCreated = schedule->creationDateTime().toMSecsSinceEpoch();
    qint64 epochTimeDateModified = schedule->lastModificationdateTime().toMSecsSinceEpoch();
    qint64 epochTimeDateDeleted = schedule->deletionDateTime().toMSecsSinceEpoch();
    QString content = schedule->content()
                            .replace("'","''")
                            .replace(QChar('\x0'), emptyStr);
    qint64 epochTimeDateStart = schedule->startDateTime().toMSecsSinceEpoch();
    qint64 epochTimeDateEnd = schedule->endDateTime().toMSecsSinceEpoch();
    QString mdContent = schedule->mdContent()
                            .replace("'","''")
                            .replace(QChar('\x0'), emptyStr);

    QString queryStr = QString("INSERT INTO deleted_schedules "
                               "VALUES (%1, %2, %3, %4, '%5', %6, %7, '%8', '%9');")
                               .arg(id)
                               .arg(epochTimeDateCreated)
                               .arg(epochTimeDateModified)
                               .arg(epochTimeDateDeleted)
                               .arg(content)
                               .arg(epochTimeDateStart)
                               .arg(epochTimeDateEnd)
                               .arg(notecolor)
                               .arg(mdContent);

    query.exec(queryStr);
    return (query.numRowsAffected() == 1);
}

void DBManager::onSchedulesListRequested()
{
    int noteCounter;
    QList<ScheduleData *> noteList;

    noteCounter = getLastRowID();
    noteList    = getAllSchedules();

    emit notesReceived(noteList, noteCounter);
}

void DBManager::onOpenDBManagerRequested(QString path, bool doCreate)
{
    open(path, doCreate);
}

void DBManager::onCreateUpdateRequested(ScheduleData* schedule)
{
    bool exists = isScheduleExist(schedule);

    if(exists){
        qDebug() << "DBManager::updateSchedule";
        updateSchedule(schedule);
    }
    else{
        qDebug() << "DBManager::addSchedule";
        addSchedule(schedule);
    }
}

void DBManager::onDeleteScheduleRequested(ScheduleData* schedule)
{
    qDebug() << "receive requestDeleteSchedule :" << __FILE__ << "当前函数 :" << __FUNCTION__ << "当前行号 :" << __LINE__;
    removeSchedule(schedule);
}

void DBManager::onImportSchedulesRequested(QList<ScheduleData *> scheduleList)
{
    QSqlDatabase::database().transaction();
    for(ScheduleData* schedule : scheduleList)
        addSchedule(schedule);
    QSqlDatabase::database().commit();
}

void DBManager::onRestoreSchedulesRequested(QList<ScheduleData*> scheduleList) {
    this->permanantlyRemoveAllSchedules();
    this->onImportSchedulesRequested(scheduleList);
}

void DBManager::onExportSchedulesRequested(QString fileName)
{
    QList<ScheduleData *> noteList;
    QFile file(fileName);
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);
#if QT_VERSION >= QT_VERSION_CHECK(5, 6, 0)
    out.setVersion(QDataStream::Qt_5_6);
#elif QT_VERSION >= QT_VERSION_CHECK(5, 4, 0)
    out.setVersion(QDataStream::Qt_5_4);
#elif QT_VERSION >= QT_VERSION_CHECK(5, 2, 0)
    out.setVersion(QDataStream::Qt_5_2);
#endif
    noteList = getAllSchedules();
    out << noteList;
    file.close();

    qDeleteAll(noteList);
    noteList.clear();
}

void DBManager::onMigrateSchedulesRequested(QList<ScheduleData *> scheduleList)
{
    QSqlDatabase::database().transaction();
    for(ScheduleData* schedule : scheduleList)
        migrateSchedule(schedule);
    QSqlDatabase::database().commit();

    qDeleteAll(scheduleList);
    scheduleList.clear();
}

void DBManager::onMigrateTrashRequested(QList<ScheduleData *> scheduleList)
{
    QSqlDatabase::database().transaction();
    for(ScheduleData* schedule : scheduleList)
        migrateTrash(schedule);
    QSqlDatabase::database().commit();

    qDeleteAll(scheduleList);
    scheduleList.clear();
}

void DBManager::onForceLastRowIndexValueRequested(int index)
{
    forceLastRowIndexValue(index);
}
