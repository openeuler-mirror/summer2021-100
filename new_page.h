#ifndef NEW_PAGE_H
#define NEW_PAGE_H

#include <QWidget>

namespace Ui {
class new_page;
}

class new_page : public QWidget
{
    Q_OBJECT

public:
    explicit new_page(QWidget *parent = nullptr);
    ~new_page();

private:

    Ui::new_page *ui;

private slots:

    void on_no_Button_clicked();

signals:
};

#endif // NEW_PAGE_H
