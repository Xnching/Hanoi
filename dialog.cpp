#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    this->setWindowTitle(QString::fromLocal8Bit("����!Υ�����!"));
}

Dialog::~Dialog()
{
    delete ui;
}
