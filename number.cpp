#include "number.h"
#include "ui_number.h"

number::number(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::number)
{
    ui->setupUi(this);
    this->setWindowTitle(QString::fromLocal8Bit("请输入数字"));
    //Qt中最经典的信号与槽
    connect(ui->lineEdit, SIGNAL(returnPressed()), this, SLOT(save()));
}

number::~number()
{
    delete ui;
    delete g1;
}

void number::closeEvent(QCloseEvent *)
 {
     emit back();
 }
void number::save()     //用来实现按下Enter键之后能够自动跳转游戏界面并获取数字
{
    if(showorgame){
        g1->showor=1;
        this->Number = ui->lineEdit->text().toInt();
        this->hide();
        emit numberEntered(Number);
    }
    else {
        this->Number = ui->lineEdit->text().toInt();
        this->hide();
        emit numberEntered(Number);
    }
}
void number::on_pushButton_clicked()
{
    //showorgame用来记录是从开始游戏按钮过来的还是从游戏演示按扭过来的
    if(showorgame){    //若从游戏演示按扭过来的就把游戏界面里的判断设为1
        g1->showor=1;
        this->Number = ui->lineEdit->text().toInt();
        this->hide();
        emit numberEntered(Number);       //给游戏界面发出数字信号,告诉其是多少数字
    }
    else {
        this->Number = ui->lineEdit->text().toInt();
        this->hide();
        emit numberEntered(Number);
    }
}

