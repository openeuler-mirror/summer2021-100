#ifndef SCHEDULEDATA_H
#define SCHEDULEDATA_H

#include <QObject>
#include <QDateTime>
#include "new_page.h"

class ScheduleData : public QObject
{
Q_OBJECT

public:
    ScheduleData(QObject *parent = Q_NULLPTR);

    int id() const;
    void setId(const int& id);

    int scheduleColor() const;
    void setScheduleColor(const int &notecolor);

    QDateTime lastModificationdateTime() const;
    void setLastModificationDateTime(const QDateTime &lastModificationdateTime);

    QDateTime creationDateTime() const;
    void setCreationDateTime(const QDateTime& creationDateTime);

    QDateTime startDateTime() const;
    void setStartDateTime(const QDateTime& startDateTime);

    QDateTime endDateTime() const;
    void setEndDateTime(const QDateTime& endDateTime);

    QString content() const;
    void setContent(const QString &content);

    QString mdContent() const;
    void setMdContent(const QString &mdContent);

    bool isModified() const;
    void setModified(bool isModified);

    bool isSelected() const;
    void setSelected(bool isSelected);

    int scrollBarPosition() const;
    void setScrollBarPosition(int scrollBarPosition);

    QDateTime deletionDateTime() const;
    void setDeletionDateTime(const QDateTime& deletionDateTime);
    new_page *m_schedule;

private:
    int m_id;
    int m_notecolor;
    QDateTime m_lastModificationDateTime;
    QDateTime m_creationDateTime;
    QDateTime m_deletionDateTime;
    QDateTime m_startDateTime;
    QDateTime m_endDateTime;
    QString m_content;
    QString m_mdContent;
    bool m_isModified;
    bool m_isSelected;
    int m_scrollBarPosition;
};

QDataStream &operator<<(QDataStream &stream, const ScheduleData* scheduleData);
QDataStream &operator>>(QDataStream &stream, ScheduleData *&scheduleData);

#endif // SCHEDULEDATA_H
