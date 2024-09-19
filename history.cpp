#include "history.h"
#include "ui_history.h"

history::history(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::history)
{
    ui->setupUi(this);
    this->setWindowTitle(QString::fromLocal8Bit("历史记录"));

    qfs.setFileName("record.txt");
    //读取文件
    if (!qfs.open(QIODevice::ReadOnly | QIODevice::Text)) {
        //失败
        qDebug() << "无法打开文件，错误信息：" << qfs.errorString();
        qDebug() << "当前工作目录：" << QDir::currentPath();
        exit(0);
    }
    //将各个历史最佳纪录添加到容器中
    for (int i=0;i<6;i++) {
        QString line = qfs.readLine();
        line.remove("\n");
        v_step_min[i] = line.toInt();
    }
    for (int i=0;i<6;i++) {
        QString line = qfs.readLine();
        line.remove("\n");
        v_time_min[i] = QLocale(QLocale::Chinese).toTime(line,"hh:mm:ss"); //从字符串转换为QTime
    }
    qfs.close();

    if (!qfs.open(QIODevice::ReadOnly | QIODevice::Text)) {
        //失败
        qDebug() << "无法打开文件，错误信息：" << qfs.errorString();
        qDebug() << "当前工作目录：" << QDir::currentPath();
        exit(0);
    }
    //数据读取到容器中,再把容器内容展示在textBrowser上
    while(!qfs.atEnd())
    {
        QByteArray array = qfs.readLine();
        QString str(array);
       // qDebug()<< str;
        data.append(str);
    }
    ui->textBrowser->setText(data);

    qfs.close();
}

std::vector<int> history::v_step_min = std::vector<int>(5,1000);
std::vector<QTime> history::v_time_min = std::vector<QTime>(5, QTime(1, 0, 0));

history::~history()
{
    delete ui;
}

void history::closeEvent(QCloseEvent *)
{
     emit back3();
}

void history::on_pushButton_clicked()
{
    this->hide();
    emit back3();
}

void history::set(){        //更新最佳历史记录的函数
    linelist.clear();
    if (!qfs.open(QIODevice::ReadOnly | QIODevice::Text)) {
        //失败
        qDebug() << "无法打开文件，错误信息：" << qfs.errorString();
        qDebug() << "当前工作目录：" << QDir::currentPath();
        exit(0);
    }
    //把所有信息读取到一个容器中,然后更改容器内的信息来达到更改文件指定前六行的最佳记录
    while (!qfs.atEnd()) {
        QString line = qfs.readLine();
        linelist.append(line);
    }

    qfs.close();
    for (int i=0;i<6;i++) {
        linelist[i]=QString::number(v_step_min[i])+"\n";
    }
    for (int i=0;i<6;i++) {
        linelist[i+6]=v_time_min[i].QTime::toString()+"\n";
    }
    if (!qfs.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&qfs);
    out.setCodec("UTF-8");
    foreach (QString line, linelist) {
        out<<line;
    }
    qfs.close();
}

//槽函数,接受game发出的信号实时添加新的记录
void history::game_step_time(int Number,int step,QTime time,bool wlema){
    //更新各个容器的记录
    if(step<=v_step_min[Number-1]){
        v_step_min[Number-1]=step;
        set();
    }
    if(time<=v_time_min[Number-1]){
        v_time_min[Number-1]=time;
        set();
    }
    QString s;
    if(wlema)
        s = "which:"+QString::number(Number)+",step:"+QString::number(step)+",time:"+time.toString()+",win!\n";
    else
        s = "which:"+QString::number(Number)+",step:"+QString::number(step)+",time:"+time.toString()+",lose!\n";

    if(!qfs.open(QIODevice::Append|QIODevice::Text)){
        //失败
        qDebug() << "无法打开文件，错误信息：" << qfs.errorString();
        qDebug() << "当前工作目录：" << QDir::currentPath();
        exit(0);
    }
    QByteArray bytes = s.toUtf8();
    qfs.write(bytes);
    qfs.close();
}

