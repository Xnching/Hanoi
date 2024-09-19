#include "number.h"
#include "ui_number.h"

number::number(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::number)
{
    ui->setupUi(this);
    this->setWindowTitle(QString::fromLocal8Bit("����������"));
    //Qt�������ź����
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
void number::save()     //����ʵ�ְ���Enter��֮���ܹ��Զ���ת��Ϸ���沢��ȡ����
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
    //showorgame������¼�Ǵӿ�ʼ��Ϸ��ť�����Ļ��Ǵ���Ϸ��ʾ��Ť������
    if(showorgame){    //������Ϸ��ʾ��Ť�����ľͰ���Ϸ��������ж���Ϊ1
        g1->showor=1;
        this->Number = ui->lineEdit->text().toInt();
        this->hide();
        emit numberEntered(Number);       //����Ϸ���淢�������ź�,�������Ƕ�������
    }
    else {
        this->Number = ui->lineEdit->text().toInt();
        this->hide();
        emit numberEntered(Number);
    }
}

