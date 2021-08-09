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
    QString active_index = "CREATE UNIQUE INDEX active_index on active_notes (id ASC);";
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
