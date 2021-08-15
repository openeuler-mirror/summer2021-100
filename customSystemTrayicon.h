/**
 * from https://blog.csdn.net/csnd_ayo/article/details/56004234
 * from https://blog.csdn.net/csnd_ayo/article/details/56004234
 * from https://blog.csdn.net/csnd_ayo/article/details/56004234
 * */

#ifndef CUSTOMSYSTEMTRAYICON_H
#define CUSTOMSYSTEMTRAYICON_H

#include <QSystemTrayIcon>
#include <QWidget>

class QAction;
class QMenu;

class customSystemTrayIcon : public QWidget
{
    Q_OBJECT

public:
    static customSystemTrayIcon* instance(QWidget* _this = nullptr);

    void setTray(const QString& _name,
                 const QString& _icon,
                 const QString& _describe);
    void setTray(const QString& _name,
                 const QString& _icon);
    void setTray(const QString& _name);

    void showTray(void);
    void hideTray(void);

private:
    explicit customSystemTrayIcon(QWidget *parent = 0);

    void init(void);
    void initMenu(void);
    void initTray(void);
    void initSignal(void);

private slots:
     void trayActivatedEvent(QSystemTrayIcon::ActivationReason _action);

protected:
     QString trayDescribe_;

private:
     // 隐藏按钮(菜单)
     QAction* hideAction_ = nullptr;
     // 还原按钮(菜单)
     QAction* restoreAction_ = nullptr;
     // 退出按钮(菜单)
     QAction* quitAction_ = nullptr;
     // 根菜单
     QMenu* menu_ = nullptr;
     // 托盘实例
     QSystemTrayIcon* trayObject_ = nullptr;
     // 实例
     static customSystemTrayIcon* self_;
};

#endif // CUSTOMSYSTEMTRAYICON_H
