#include "victory.h"
#include "ui_victory.h"

victory::victory(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::victory)
{
    ui->setupUi(this);
    this->setWindowTitle(QString::fromLocal8Bit("ÓÎÏ·Ê¤Àû!"));

}

victory::~victory()
{
    delete ui;
}

void victory::on_pushButton_clicked()
{
    this->close();
}


