

#include <QApplication>
#include <QtGui>
#include <QMenu>
#include <QAction>

#ifndef QT_NO_DEBUG
#include <QMessageBox>
#endif

#include "customSystemTrayicon.h"
#include <qdebug.h>




customSystemTrayIcon* customSystemTrayIcon::self_ = nullptr;


customSystemTrayIcon::customSystemTrayIcon(QWidget *parent)
    : QWidget(parent) {
    init();
}

void customSystemTrayIcon::setTray(const QString &_name,
                                   const QString &_icon,
                                   const QString &_describe) {
    setTray(_name,_icon);
    trayDescribe_ = _describe;
}

void customSystemTrayIcon::setTray(const QString &_name,
                                   const QString &_icon) {
    setTray(_name);
    trayObject_->setIcon(QIcon(_icon));
}

void customSystemTrayIcon::setTray(const QString &_name) {
    trayObject_->setToolTip(_name);
}

void customSystemTrayIcon::showTray()
{
    trayObject_->show();
    trayObject_->showMessage(trayObject_->toolTip(),trayDescribe_,
                            QIcon(":/myImage/calendar.svg"), 5000);
}

void customSystemTrayIcon::hideTray()
{
    trayObject_->hide();
}

customSystemTrayIcon* customSystemTrayIcon::instance(QWidget *_this)
{
    if (self_ == nullptr) {
        if (_this != nullptr) {
            self_ = new customSystemTrayIcon(_this);
        }
        else {
#ifndef QT_NO_DEBUG
            QMessageBox::critical(nullptr,
                   "警告","不符合要求的托盘操作代码");
#endif
            return nullptr;
        }
    }
    return self_;
}

void customSystemTrayIcon::trayActivatedEvent(
        QSystemTrayIcon::ActivationReason _action) {
    switch(_action)
    {
    case QSystemTrayIcon::Unknown:
        trayObject_->showMessage(trayObject_->toolTip(),trayDescribe_,
                                QIcon(":/myImage/calendar.svg"), 5000);
        break;
    // 右键单击
    case QSystemTrayIcon::Context:
        break;
    // 点击
    case QSystemTrayIcon::Trigger:
        break;
    // 双击
    case QSystemTrayIcon::DoubleClick:
        qDebug()<<"111111";
        this->parentWidget()->showNormal();
        break;
    // 中键点击
    case QSystemTrayIcon::MiddleClick:
        this->parentWidget()->hide();
        break;
    default:
        break;
    }
}

void customSystemTrayIcon::init() {
    //判断系统是否支持系统托盘图标
    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
#ifndef QT_NO_DEBUG
            QMessageBox::critical(nullptr,
                   "警告","当前系统不支持系统托盘");
#endif
        return;
    }

    // 托盘对象
    trayObject_ = new QSystemTrayIcon(this);
    // 隐藏按钮（菜单）
    hideAction_ = new QAction("隐 藏",this);
    // 还原按钮（菜单）
    restoreAction_ = new QAction("还 原",this);
    // 退出按钮（菜单）
    quitAction_ = new QAction("退 出",this);
    // 根菜单
    menu_ = new QMenu((QWidget*)QApplication::desktop());

    initMenu();
    initTray();
    initSignal();
}

void customSystemTrayIcon::initMenu()
{
    // 设置菜单图标
//    hideAction_->setIcon(QIcon(":/icon/hide"));
//    restoreAction_->setIcon(QIcon(":/icon/restore"));
//    quitAction_->setIcon(QIcon(":/icon/quit"));

    // 组织菜单顺序
    menu_->addAction(hideAction_);
    menu_->addAction(restoreAction_);
    //加入一个分离符
    menu_->addSeparator();
    menu_->addAction(quitAction_);
}

void customSystemTrayIcon::initTray()
{
    trayObject_->setIcon(QIcon(":/myImage/calendar.svg"));   //设置图标图片
    //setWindowIcon(QIcon(":/myImage/calendar.svg"));  //把图片设置到窗口上
    trayObject_->setToolTip("日程管理");    //托盘时，鼠标放上去的提示信息
    // 设置托盘右键菜单
    trayObject_->setContextMenu(menu_);
    trayObject_->hide();
}

void customSystemTrayIcon::initSignal()
{
    // 菜单项功能关联
    connect(hideAction_,SIGNAL(triggered()),
                  parentWidget(),SLOT(hide()));
    connect(restoreAction_,SIGNAL(triggered()),
                  parentWidget(),SLOT(showNormal()));
    connect(quitAction_,SIGNAL(triggered()),
                  qApp,SLOT(quit()));

    // 托盘激活事件(点击、双击、滚轮点击)
    connect(trayObject_,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this,SLOT(trayActivatedEvent(QSystemTrayIcon::ActivationReason)));
}

