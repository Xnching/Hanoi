#include "history.h"
#include "ui_history.h"

history::history(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::history)
{
    ui->setupUi(this);
    this->setWindowTitle(QString::fromLocal8Bit("��ʷ��¼"));

    qfs.setFileName("record.txt");
    //��ȡ�ļ�
    if (!qfs.open(QIODevice::ReadOnly | QIODevice::Text)) {
        //ʧ��
        qDebug() << "�޷����ļ���������Ϣ��" << qfs.errorString();
        qDebug() << "��ǰ����Ŀ¼��" << QDir::currentPath();
        exit(0);
    }
    //��������ʷ��Ѽ�¼��ӵ�������
    for (int i=0;i<6;i++) {
        QString line = qfs.readLine();
        line.remove("\n");
        v_step_min[i] = line.toInt();
    }
    for (int i=0;i<6;i++) {
        QString line = qfs.readLine();
        line.remove("\n");
        v_time_min[i] = QLocale(QLocale::Chinese).toTime(line,"hh:mm:ss"); //���ַ���ת��ΪQTime
    }
    qfs.close();

    if (!qfs.open(QIODevice::ReadOnly | QIODevice::Text)) {
        //ʧ��
        qDebug() << "�޷����ļ���������Ϣ��" << qfs.errorString();
        qDebug() << "��ǰ����Ŀ¼��" << QDir::currentPath();
        exit(0);
    }
    //���ݶ�ȡ��������,�ٰ���������չʾ��textBrowser��
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

void history::set(){        //���������ʷ��¼�ĺ���
    linelist.clear();
    if (!qfs.open(QIODevice::ReadOnly | QIODevice::Text)) {
        //ʧ��
        qDebug() << "�޷����ļ���������Ϣ��" << qfs.errorString();
        qDebug() << "��ǰ����Ŀ¼��" << QDir::currentPath();
        exit(0);
    }
    //��������Ϣ��ȡ��һ��������,Ȼ����������ڵ���Ϣ���ﵽ�����ļ�ָ��ǰ���е���Ѽ�¼
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

//�ۺ���,����game�������ź�ʵʱ����µļ�¼
void history::game_step_time(int Number,int step,QTime time,bool wlema){
    //���¸��������ļ�¼
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
        //ʧ��
        qDebug() << "�޷����ļ���������Ϣ��" << qfs.errorString();
        qDebug() << "��ǰ����Ŀ¼��" << QDir::currentPath();
        exit(0);
    }
    QByteArray bytes = s.toUtf8();
    qfs.write(bytes);
    qfs.close();
}

