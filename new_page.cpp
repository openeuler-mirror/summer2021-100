#include "new_page.h"
#include "ui_new_page.h"

new_page::new_page(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::new_page)
{
    ui->setupUi(this);
}

new_page::~new_page()
{
    delete ui;
}

void new_page::on_no_Button_clicked()
{
    this->close();
}

