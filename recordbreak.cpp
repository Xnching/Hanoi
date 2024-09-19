#include "recordbreak.h"
#include "ui_recordbreak.h"

recordBreak::recordBreak(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::recordBreak)
{
    ui->setupUi(this);
    this->setWindowTitle(QString::fromLocal8Bit("´òÆÆ¼ÍÂ¼!"));
}

recordBreak::~recordBreak()
{
    delete ui;
}

void recordBreak::on_pushButton_clicked()
{
    this->close();
}
