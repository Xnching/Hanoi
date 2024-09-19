#include "explain.h"
#include "ui_explain.h"

explain::explain(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::explain)
{
    ui->setupUi(this);
    this->setWindowTitle(QString::fromLocal8Bit("ÓÎÏ·ËµÃ÷"));

}

explain::~explain()
{
    delete ui;
}
void explain::closeEvent(QCloseEvent *)
 {
     emit back();
 }

void explain::on_pushButton_clicked()
{
    this->hide();
    emit back();
}
