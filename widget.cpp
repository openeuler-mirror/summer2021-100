#include "widget.h"
#include "ui_widget.h"

#include <QDebug>

Widget::Widget(QWidget *parent):
    QWidget(parent),
    ui(new Ui::schedule)
  , dateString("")
  , date(new QDate())
  , time(new QTime())
  , mTimerRefresh(new QTimer(this))
  , np(Q_NULLPTR)
  , m_settingsDatabase(Q_NULLPTR)
  , m_dbManager(Q_NULLPTR)
  , m_dbThread(Q_NULLPTR)
  , m_scheduleCounter(0)
  , m_trashCounter(0)
{
    /** 这里应为翻译部分代码
     *
     *
     *
     *
    **/

    ui->setupUi(this);

    setupDatabases();
    kyScheduleInit();
    kyScheduleConn();
    QTimer::singleShot(200,this, SLOT(InitData()));
}

Widget::~Widget()
{
    delete ui;
    m_dbThread->quit();
    m_dbThread->wait();
    delete m_dbThread;
}

void Widget::initializeSettingsDatabase()
{
    if(m_settingsDatabase->value(QStringLiteral("version"), "NULL") == "NULL")
        m_settingsDatabase->setValue(QStringLiteral("version"), qApp->applicationVersion());

    if(m_settingsDatabase->value(QStringLiteral("windowGeometry"), "NULL") == "NULL"){
        int initWidth = 590;
        int initHeight = 630;
        QPoint center = qApp->desktop()->geometry().center();
        QRect rect(center.x() - initWidth/2, center.y() - initHeight/2, initWidth, initHeight);
        setGeometry(rect);
        m_settingsDatabase->setValue(QStringLiteral("windowGeometry"), saveGeometry());
    }
}

void Widget::setupDatabases()
{
    m_settingsDatabase = new QSettings(QSettings::IniFormat, QSettings::UserScope,
                                       QStringLiteral("kylin-schedule"), QStringLiteral("Settings"), this);
    m_settingsDatabase->setFallbacksEnabled(false);
    initializeSettingsDatabase();

    bool doCreate = false;
    QFileInfo fi(m_settingsDatabase->fileName());
    QDir dir(fi.absolutePath());
    bool folderCreated = dir.mkpath(QStringLiteral("."));
    if(!folderCreated)
        qFatal("ERROR: Can't create settings folder : %s", dir.absolutePath().toStdString().c_str());

    QString noteDBFilePath(dir.path() + QDir::separator() + QStringLiteral("kySchedules.db"));

    if(!QFile::exists(noteDBFilePath)){
        QFile noteDBFile(noteDBFilePath);
        if(!noteDBFile.open(QIODevice::WriteOnly))
            qFatal("ERROR : Can't create database file");

        noteDBFile.close();
        doCreate = true;
    }

    m_dbManager = new DBManager;
    m_dbThread = new QThread;
    m_dbThread->setObjectName(QStringLiteral("dbThread"));
    m_dbManager->moveToThread(m_dbThread);
    connect(m_dbThread, &QThread::started, [=](){emit requestOpenDBManager(noteDBFilePath, doCreate);});
    connect(this, &Widget::requestOpenDBManager, m_dbManager, &DBManager::onOpenDBManagerRequested);
    connect(m_dbThread, &QThread::finished, m_dbManager, &QObject::deleteLater);
    m_dbThread->start();
}


void Widget::kyScheduleInit()
{
    //年月按钮初始化
    this->dateString.append(QString::number(this->ui->calendar->readSelectDate().year()));
    this->dateString.append("年");
    this->dateString.append(QString::number(this->ui->calendar->readSelectDate().month()));
    this->dateString.append("月");

    ui->yearButton->setText(dateString);

    //设置定时器 20ms一刷新
    mTimerRefresh->start(20);
    connect(mTimerRefresh, SIGNAL(timeout()), this, SLOT(widget_refresh()));
}


void Widget::kyScheduleConn()
{
    //connect(ui->yearButton, &QPushButton::clicked, this, &Widget::on_yearButton_clicked);     编译器自动连接
    //connect(ui->newButton, &QPushButton::clicked, this, &Widget::on_newButton_clicked);       编译器自动连接



    connect(this, &Widget::requestSchedulesList,
            m_dbManager,&DBManager::onSchedulesListRequested, Qt::BlockingQueuedConnection);
    connect(this, &Widget::requestCreateUpdateSchedule,
            m_dbManager, &DBManager::onCreateUpdateRequested, Qt::BlockingQueuedConnection);
    connect(this, &Widget::requestDeleteSchedule,
            m_dbManager, &DBManager::onDeleteScheduleRequested);
    //    connect(this, &Widget::requestRestoreNotes,
    //            m_dbManager, &DBManager::onRestoreNotesRequested, Qt::BlockingQueuedConnection);
    //    connect(this, &Widget::requestImportNotes,
    //            m_dbManager, &DBManager::onImportNotesRequested, Qt::BlockingQueuedConnection);
    //    connect(this, &Widget::requestExportNotes,
    //            m_dbManager, &DBManager::onExportNotesRequested, Qt::BlockingQueuedConnection);



    //connect(this, &Widget::requestMigrateSchedules,
    //        m_dbManager, &DBManager::onMigrateSchedulesRequested, Qt::BlockingQueuedConnection);
    //connect(this, &Widget::requestMigrateTrash,
    //        m_dbManager, &DBManager::onMigrateTrashRequested, Qt::BlockingQueuedConnection);  数据库迁移部分，还没整好

    connect(this, &Widget::requestForceLastRowIndexValue,
            m_dbManager, &DBManager::onForceLastRowIndexValueRequested, Qt::BlockingQueuedConnection);

    //connect(m_dbManager, &DBManager::notesReceived, this, &Widget::loadSchedules);    展示日程，，还没整好
}


void Widget::checkMigration()       //还没整好
{
    QFileInfo fi(m_settingsDatabase->fileName());
    QDir dir(fi.absolutePath());

    QString oldNoteDBPath(dir.path() + QDir::separator() + "Schedules.ini");
    if(QFile::exists(oldNoteDBPath))
        migrateSchedule(oldNoteDBPath);

    QString oldTrashDBPath(dir.path() + QDir::separator() + "Trash.ini");
    if(QFile::exists(oldTrashDBPath))
        //migrateTrash(oldTrashDBPath);

        emit requestForceLastRowIndexValue(m_scheduleCounter);
}

void Widget::migrateSchedule(QString notePath)  //还没整好
{
    QSettings notesIni(notePath, QSettings::IniFormat);
    QStringList dbKeys = notesIni.allKeys();

    m_scheduleCounter = notesIni.value(QStringLiteral("notesCounter"), "0").toInt();
    QList<ScheduleData *> noteList;

    auto it = dbKeys.begin();
    for(; it < dbKeys.end()-1; it += 3){
        QString noteName = it->split(QStringLiteral("/"))[0];
        int id = noteName.split(QStringLiteral("_"))[1].toInt();

        // sync db index with biggest notes id
        m_scheduleCounter = m_scheduleCounter < id ? id : m_scheduleCounter;

        ScheduleData* newNote = new ScheduleData(this);
        newNote->setId(id);

        QString createdDateDB = notesIni.value(noteName + QStringLiteral("/dateCreated"), "Error").toString();
        newNote->setCreationDateTime(QDateTime::fromString(createdDateDB, Qt::ISODate));
        QString lastEditedDateDB = notesIni.value(noteName + QStringLiteral("/dateEdited"), "Error").toString();
        newNote->setLastModificationDateTime(QDateTime::fromString(lastEditedDateDB, Qt::ISODate));
        QString contentText = notesIni.value(noteName + QStringLiteral("/content"), "Error").toString();
        newNote->setContent(contentText);

        noteList.append(newNote);
    }

    if(!noteList.isEmpty())
        emit requestMigrateSchedules(noteList);

    QFile oldNoteDBFile(notePath);
    oldNoteDBFile.rename(QFileInfo(notePath).dir().path() + QDir::separator() + QStringLiteral("oldNotes.ini"));
}



//刷新时间显示
void Widget::updateTimeButton()
{
    this->ui->timeButton->setText(this->time->currentTime().toString("hh:mm:ss"));

}

//刷新日期显示
void Widget::updateYearButton()
{
    if(this->ui->calendar->readPageState() == "month_select" || this->ui->calendar->readPageState() == "year_select"){
        this->dateString.clear();
        this->dateString.append(QString::number(this->ui->calendar->readSelectDate().year()));
        this->dateString.append("年");

        ui->yearButton->setText(dateString);
    }
    else{
        this->dateString.clear();
        this->dateString.append(QString::number(this->ui->calendar->readSelectDate().year()));
        this->dateString.append("年");
        this->dateString.append(QString::number(this->ui->calendar->readSelectDate().month()));
        this->dateString.append("月");

        ui->yearButton->setText(dateString);
    }
}

void Widget::create_update_slots(ScheduleData *schedule)
{
    qDebug()<<"成功了吗？";
    emit requestCreateUpdateSchedule(schedule);
}

/**
 * slots
 **/

void Widget::InitData()
{
    QFileInfo fi(m_settingsDatabase->fileName());
    QDir dir(fi.absolutePath());
    QString oldNoteDBPath(dir.path() + QStringLiteral("/Schedules.ini"));
    QString oldTrashDBPath(dir.path() + QStringLiteral("/Trash.ini"));

    bool exist = (QFile::exists(oldNoteDBPath) || QFile::exists(oldTrashDBPath));

    if(exist){
        QProgressDialog* pd = new QProgressDialog(QStringLiteral("Migrating database, please wait."), QString(), 0, 0, this);
        pd->setCancelButton(Q_NULLPTR);
        pd->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
        pd->setMinimumDuration(0);
        pd->show();


        QFutureWatcher<void>* watcher = new QFutureWatcher<void>(this);
        connect(watcher, &QFutureWatcher<void>::finished, this, [&, pd](){
            pd->deleteLater();
            emit requestSchedulesList();
        });

        QFuture<void> migration = QtConcurrent::run(this, &Widget::checkMigration);
        watcher->setFuture(migration);

    } else {        //前面的都没整好，反正不会进迁移，会直接进这个else
        emit requestSchedulesList();
    }
}

//定时器溢出处理 画面刷新 20ms
void Widget::widget_refresh()
{
    this->ui->calendar->update();
    this->updateTimeButton();
    this->updateYearButton();
}

//点击年份显示按钮
void Widget::on_yearButton_clicked()
{
    this->ui->calendar->setPageState();

    updateYearButton();
}

//点击新建按钮
void Widget::on_newButton_clicked()
{

    np = new new_page;

    np->setWindowModality(Qt::ApplicationModal);

    connect(this->np, &new_page::sendScheduleData, this, &Widget::create_update_slots);

    np->show();
}

/**
 * signals
 * */



