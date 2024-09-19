#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    //����
    this->setWindowTitle(QString::fromLocal8Bit("��ŵ��С��Ϸ"));
}
Widget::~Widget()
{
    delete ui;
    delete e;
    delete n;
    delete h;
}
//�رհ�ť����ʵ��
void Widget::on_ExitButton_clicked()
{
    exit(0);
}
//��Ϸ��ʾ��ť����ʵ��
void Widget::on_ShowButton_clicked()
{
    //��ʾ����ʵ��ͬ��Ϸ����ʵ��,��ʵ���Ͼ����Զ���Ϸ,���ǲ�ͬ������ʷ��¼��¼��ʷ
    hide();
    n = new number();
    n->showorgame=1;
    n->g1 = new Game();
    n->show();

    //ͬ��Ϸ��ť���connect����
    connect(this->n,SIGNAL(back()),this,SLOT(show()));
    connect(n,&number::numberEntered,n->g1,&Game::set);
    connect(n->g1,SIGNAL(back1()),this,SLOT(show()));
}
//��ʷ��¼��ť����ʵ��
void Widget::on_HistoryButton_clicked()
{
    delete h;
    this->hide();
    this->h = new history();
    this->h->show();
    //��ת����ҳ��
    connect(h,SIGNAL(back3()),this,SLOT(show()));
}
//��Ϸ˵����ť����ʵ��
void Widget::on_ExplainButton_clicked()
{
    hide();
    e = new explain();
    e->show();
    //��ת����ҳ��
    connect(this->e,SIGNAL(back()),this,SLOT(show()));
}
//��ʼ��Ϸ��ť����ʵ��
void Widget::on_StartButton_clicked()
{
    delete h;
    hide();
    n = new number();
    n->showorgame=0;
    n->show();
    //��Ϸ����򿪵�ʱ��,��ʷ��¼Ҳ��,ʵ�����ݼ�¼����
    h = new history();
    n->g1 = new Game();
    //ʵ�ֵ���Ϸ����ʷ��¼�����¼
    connect(n->g1,&Game::aboutStepnTime,h,&history::game_step_time);

    //ͨ��connect������2���ļ��е��ź���ۺ���
    //ʵ�ֵ���������ҳ��ر���ת����ҳ��
    connect(this->n,SIGNAL(back()),this,SLOT(show()));
    //ʵ�ֵ���������ҳ�����Ϸҳ�洫�����������
    connect(n,&number::numberEntered,n->g1,&Game::set);
    //ʵ�ֵ���Ϸ����ر���ת����ҳ��
    connect(n->g1,SIGNAL(back1()),this,SLOT(show()));
}
