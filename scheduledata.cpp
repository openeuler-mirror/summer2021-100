#include "scheduledata.h"

ScheduleData::ScheduleData(QObject *parent)
    : QObject(parent),
      m_isModified(false),
      m_isSelected(false),
      m_scrollBarPosition(0)
{

}

int ScheduleData::id() const
{
    return m_id;
}

void ScheduleData::setId(const int &id)
{
    m_id = id;
}


int ScheduleData::scheduleColor() const
{
    return m_schedulecolor;
}

void ScheduleData::setScheduleColor(const int &schedulecolor)
{
    m_schedulecolor = schedulecolor;
}

QDateTime ScheduleData::lastModificationdateTime() const
{
    return m_lastModificationDateTime;
}

void ScheduleData::setLastModificationDateTime(const QDateTime &lastModificationdateTime)
{
    m_lastModificationDateTime = lastModificationdateTime;
}

QDateTime ScheduleData::startDateTime() const
{
    return m_startDateTime;
}

void ScheduleData::setStartDateTime(const QDateTime &startDateTime)
{
    m_startDateTime = startDateTime;
}

QDateTime ScheduleData::endDateTime() const
{
    return m_endDateTime;
}

void ScheduleData::setEndDateTime(const QDateTime &endDateTime)
{
    m_endDateTime = endDateTime;
}

QString ScheduleData::content() const
{
    return m_content;
}

void ScheduleData::setContent(const QString &content)
{
    m_content = content;
}

QString ScheduleData::mdContent() const
{
    return m_mdContent;
}

void ScheduleData::setMdContent(const QString &mdContent)
{
    m_mdContent = mdContent;
}

bool ScheduleData::isModified() const
{
    return m_isModified;
}

void ScheduleData::setModified(bool isModified)
{
    m_isModified = isModified;
}

bool ScheduleData::isSelected() const
{
    return m_isSelected;
}

void ScheduleData::setSelected(bool isSelected)
{
    m_isSelected = isSelected;
}

int ScheduleData::scrollBarPosition() const
{
    return m_scrollBarPosition;
}

void ScheduleData::setScrollBarPosition(int scrollBarPosition)
{
    m_scrollBarPosition = scrollBarPosition;
}

QDateTime ScheduleData::deletionDateTime() const
{
    return m_deletionDateTime;
}

void ScheduleData::setDeletionDateTime(const QDateTime& deletionDateTime)
{
    m_deletionDateTime = deletionDateTime;
}

QDateTime ScheduleData::creationDateTime() const
{
    return m_creationDateTime;
}

void ScheduleData::setCreationDateTime(const QDateTime&creationDateTime)
{
    m_creationDateTime = creationDateTime;
}

QDataStream &operator<<(QDataStream &stream, const ScheduleData* scheduleData) {
    return stream << scheduleData->id() << scheduleData->creationDateTime() \
                  << scheduleData->lastModificationdateTime() << scheduleData->content() \
                  << scheduleData->startDateTime() << scheduleData->endDateTime()   \
                  << scheduleData->scheduleColor() << scheduleData->mdContent();
}

QDataStream &operator>>(QDataStream &stream, ScheduleData* &scheduleData){
    scheduleData = new ScheduleData();
    int id;
    int scheduleColor;
    QDateTime lastModificationDateTime;
    QDateTime creationDateTime;
    QDateTime startDateTime;
    QDateTime endDateTime;
    QString content;
    QString mdContent;
    stream >> id >> creationDateTime >> lastModificationDateTime >> content >> startDateTime >> endDateTime >> scheduleColor >> mdContent;
    scheduleData->setId(id);
    scheduleData->setLastModificationDateTime(lastModificationDateTime);
    scheduleData->setCreationDateTime(creationDateTime);
    scheduleData->setStartDateTime(startDateTime);
    scheduleData->setEndDateTime(endDateTime);
    scheduleData->setContent(content);
    scheduleData->setScheduleColor(scheduleColor);
    scheduleData->setMdContent(mdContent);
    return stream;
}
