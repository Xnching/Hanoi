#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    //标题
    this->setWindowTitle(QString::fromLocal8Bit("汉诺塔小游戏"));
}
Widget::~Widget()
{
    delete ui;
    delete e;
    delete n;
    delete h;
}
//关闭按钮功能实现
void Widget::on_ExitButton_clicked()
{
    exit(0);
}
//游戏演示按钮功能实现
void Widget::on_ShowButton_clicked()
{
    //演示功能实现同游戏功能实现,即实际上就是自动游戏,但是不同步打开历史记录记录历史
    hide();
    n = new number();
    n->showorgame=1;
    n->g1 = new Game();
    n->show();

    //同游戏按钮里的connect函数
    connect(this->n,SIGNAL(back()),this,SLOT(show()));
    connect(n,&number::numberEntered,n->g1,&Game::set);
    connect(n->g1,SIGNAL(back1()),this,SLOT(show()));
}
//历史记录按钮功能实现
void Widget::on_HistoryButton_clicked()
{
    delete h;
    this->hide();
    this->h = new history();
    this->h->show();
    //跳转回主页面
    connect(h,SIGNAL(back3()),this,SLOT(show()));
}
//游戏说明按钮功能实现
void Widget::on_ExplainButton_clicked()
{
    hide();
    e = new explain();
    e->show();
    //跳转回主页面
    connect(this->e,SIGNAL(back()),this,SLOT(show()));
}
//开始游戏按钮功能实现
void Widget::on_StartButton_clicked()
{
    delete h;
    hide();
    n = new number();
    n->showorgame=0;
    n->show();
    //游戏界面打开的时候,历史记录也打开,实现数据记录传输
    h = new history();
    n->g1 = new Game();
    //实现的游戏给历史记录传输记录
    connect(n->g1,&Game::aboutStepnTime,h,&history::game_step_time);

    //通过connect来链接2个文件中的信号与槽函数
    //实现的输入数字页面关闭跳转回主页面
    connect(this->n,SIGNAL(back()),this,SLOT(show()));
    //实现的输入数字页面给游戏页面传入输入的数字
    connect(n,&number::numberEntered,n->g1,&Game::set);
    //实现的游戏界面关闭跳转回主页面
    connect(n->g1,SIGNAL(back1()),this,SLOT(show()));
}
