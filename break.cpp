#include "break.h"
#include "ui_break.h"

break1::break1(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::break1)
{
    ui->setupUi(this);
}

break1::~break1()
{
delete ui;
}
