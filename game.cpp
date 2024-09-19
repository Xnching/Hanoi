#include "game.h"
#include "ui_game.h"

Game::Game(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Game)
{

    //չʾUI
    ui->setupUi(this);
    this->setWindowTitle(QString::fromLocal8Bit("��ŵ����Ϸ"));
    this->hide();

    //���ü�ʱ��
    timer = new QTimer;
    time = new QTime;
    ui->lcdNumber->setDigitCount(8);
    initTime();
    //����ʱ���connect�������connect�ڴ˴�,��ʼ��connect������button������set()��
    connect(timer,SIGNAL(timeout()),this,SLOT(updateTime()));
    connect(this,&Game::end,this,&Game::end_time);

    //����ʱ������
    v_time_limit.push_back(QTime(0,0,3));//5,9,28,75,225,675
    v_time_limit.push_back(QTime(0,0,6));
    v_time_limit.push_back(QTime(0,0,15));
    v_time_limit.push_back(QTime(0,0,35));
    v_time_limit.push_back(QTime(0,1,45));
    v_time_limit.push_back(QTime(0,4,15));
}

Game::~Game()
{
    delete ui;
    delete button_1;
    delete button_2;
    delete button_3;
    delete button_4;
    delete button_5;
    delete button_6;
    delete timer;
    delete time;
    delete vi;
    delete dia;
}

void Game::set(int NUMBER){
    //���ܵ���д�ɺ���,��Ҫ�ڹ��캯����д
    //��������λ����x=900,�ڶ�������x=581,��һ������x=260,��������y=635
    //�������Ƿŵ�һ��������,�������Ը���Number��������������

    Numberg = NUMBER;
    if(NUMBER<=0||NUMBER>=7)Numberg = 3;

    time_limit = new QLabel(this);
    time_limit->setText(QString::fromLocal8Bit("��ʱ:")+v_time_limit[Numberg-1].toString());
    time_limit->move(15,215);
    //����Ҫ��new����Ӳ�Ȼһֱ�����쳣����
    button_1 = new MPushButton(this);
    button_2 = new MPushButton(this);
    button_3 = new MPushButton(this);
    button_4 = new MPushButton(this);
    button_5 = new MPushButton(this);
    button_6 = new MPushButton(this);
    v.append(button_1);
    v.append(button_2);
    v.append(button_3);
    v.append(button_4);
    v.append(button_5);
    v.append(button_6);
    for(int i =0;i<6;i++){
        v[i]->hide();
    }

    //��ʼ��ʼ����������,ʹ���ʼʱ���ڵ�һ����������
    set_Button();

    //�����ź����,Ҫ�ڴ˴�����,��������������亯��������,��ΪGame��������
    //�����ӱ��ƶ���ʱ��͵��ÿ���κ���

    for (int i=0;i<6;i++) {
        connect(v[i],&MPushButton::ButtonMove,this,&Game::ButtonMove_Slot);
        connect(v[i],&MPushButton::wrong1,this,&Game::warning1);
    }

    //��button��connect����Ҫ���ڴ˴�
    connect(button_1,&MPushButton::win,this,&Game::whether_win);
    connect(button_1,&MPushButton::start1,this,&Game::start_time);
    this->show();
    if(showor){
        move_hanoi(Numberg,260,581,900);
        startMove();
    }
    vi = new victory();
    rb = new recordBreak();
}

void Game::initTime(){          //��ʼ��ʱ��
    time->setHMS(0,0,0);
    ui->lcdNumber->display(time->toString("hh::mm::ss"));
}

void Game::updateTime(){        //����ʱ��
    *time = time->addSecs(1);
    ui->lcdNumber->display(time->toString("hh::mm::ss"));
}

void Game::start_time(){        //��ʼ��ʱ
    if(showor)return ;
    timer->start(1000);
}

void Game::end_time(){
    if(showor)return ;
    timer->stop();
    howTime  = time->toString();
    emit aboutStepnTime(Numberg,step,*time,wlema);

}

void Game::closeEvent(QCloseEvent *)    //���ҳ��Ĺر��¼�
{
     emit back1();
}

void Game::on_returnButton_clicked()    //���ذ�ť��ʵ��
{
    emit back1();
    this->close();
}

void MPushButton::mouseMoveEvent(QMouseEvent *event){   //��д�ƶ��¼�
    //�����ȵ���Զ���ؼ�������,�ƶ��¼��Ż�ʹ�Զ���ؼ�������ƶ�,��p���������ж��Ƿ���������
    if (p) {
        const QPoint delta = event->globalPos() - mouse;
        move(x()+delta.x(), y()+delta.y());
        //����λ��
        mouse = event->globalPos();
    }

    QPushButton::mouseMoveEvent(event);
}

void MPushButton::mousePressEvent(QMouseEvent *event){  //��д����¼�

    //����������Զ���ؼ�,��֤���Լ���ʼ���,����¼���λ��
    if (event->button() == Qt::LeftButton) {
        p = true;
        mouse = event->globalPos();
    }
    if(!startLeMa){
        startLeMa = 1;
        emit start1();
    }
    emit wrong1();
    QPushButton::mousePressEvent(event);
}

void MPushButton::mouseReleaseEvent(QMouseEvent *event){    //��д�ͷ��¼�
    //�ͷž͸���p��״̬
    if (event->button() == Qt::LeftButton) {
        p = false;
    }
    emit ButtonMove();
    emit win();
    QPushButton::mouseReleaseEvent(event);
}
//��Press����,�ж��Ƿ��϶��ķǶ��˰�ť
void Game::warning1(){
    for(int i=0;i<Numberg;i++){
        if(v[i]==sender()){     //�ҵ����϶��İ�ť
            int xi = v[i]->pos().x();
            int yi = v[i]->pos().y();
            int width = v[i]->width();
            for (int j=0;j<i;j++) {//��С����һ��һ����,���Ƿ����ڱ��ϵö���ť����,����о�û���϶�
                if(v[j]->pos().y()<yi&&
                        xi-v[j]->pos().x()>=-width&&
                            xi-v[j]->pos().x()<=v[j]->width()){
                    dia  = new Dialog();
                    //�����þ�����뱻������ܲ�����Ϸ����
                    dia->setWindowModality(Qt::ApplicationModal);
                    dia->show();
                    break;  //��������������
                }
            }
            break;
        }
    }
}
void Game::ButtonMove_Slot(){       //ʵ�ֿ���Ĳۺ���

    this->step++;
    QString s = QString::number(step);
    int i=1;
    //ʵ���Զ���������
    for (i=1;i<=Numberg;i++){

        int x = v[i-1]->pos().x();
        int y = v[i-1]->pos().y();

        if(v[i-1]==sender()){           //һ��һ���ҿ����ĸ����ӷ����źŵ�
            if( x >= 661  ){    //һ��һ���ж����ĸ����ӽ�,���ĸ������ƶ����ĸ���������
                v[i-1]->move(900-(v[i-1]->width()/2),y);
                break;
            }else if ( x >= 341 && x <= 660 ) {
                v[i-1]->move(581-(v[i-1]->width()/2),y);
                break;
            }else if ( x <= 340 ) {
                v[i-1]->move(260-(v[i-1]->width()/2),y);
                break;
            }
        }
    }

    //warning2,��Release����,�ж��Ƿ��Ǵ������Ҫ�ŵ�С��������
    int xi = v[i-1]->pos().x();
    int yi = v[i-1]->pos().y();
    int width = v[i-1]->width();
    for (int j=1;j<i;j++) {
        if(yi<v[j-1]->pos().y()&&
                xi-v[j-1]->pos().x()>=-width&&
                    xi-v[j-1]->pos().x()<=v[j-1]->width()){
            dia  = new Dialog();
            dia->setWindowModality(Qt::ApplicationModal);
            dia->show();
            return ;
        }
    }

    zhuzi_kaoqi();      //ʵ������֮����Զ�����ĺ���
    ui->label_step->setText("step: "+s);
}

void Game::zhuzi_kaoqi(){
    //ʵ������֮���Զ�����
    for(int i=1;i<=Numberg;i++){     //һ��һ���ҿ����ĸ����ӷ����źŵ�,iΪ���ƶ�������

        if(v[i-1]==sender()){
            int xi = v[i-1]->pos().x();
            int yi = v[i-1]->pos().y();
            int width = v[i-1]->width();

            //jһ��һ����С����ƥ��,��������С������,��ôС�ľ�move���������
            for (int j=i+1;j<=Numberg+1;j++) {
                //����ifʵ���Ͼ�ֻ�����ж�����i���������Ѻõ����ӵ�λ���Ƿ���Ͽ���Ҫ��
                if( j<=Numberg&&
                       yi<=v[j-1]->pos().y()&&
                           xi-v[j-1]->pos().x()>=-width&&
                               xi-v[j-1]->pos().x()<=v[j-1]->width() ) {
                    v[i-1]->move(xi,v[j-1]->pos().y()-v[i-1]->height());
                    break;      //ͨ��breakʹ�Լ���Զ�ڴ����Լ���j��Щ����С������
                }
                //���i����û�д��,��ô���ƶ���������
                //ע�������&&����ļ���||����Ҫ������һ��,��Ϊ������һ��&&��ǰ���
                else if( j == (Numberg+1) &&
                            ( xi==(581-width/2)||
                                 xi==(260-width/2)||
                                    xi==(900-width/2) )  ) {
                    v[i-1]->move(xi,635);
                    break;
                }
            }
            break;      //breakֱ�Ӳ���ȥ�Һ���İ�ť�ǲ��Ƿ����źŵ�
        }
    }
}
//ʵ�ֵ��ж�ʤ���Ĳۺ���
void Game::whether_win(){
    if(showor)return ;
    int winMa = 0;

    //ֻ���ж���С�������Ƿ����ұ�����������,Ȼ�����С����һ��,���αȽ��Ƿ�С�Ķ��ڴ������
    if(v[0]->pos().x()>=350){
        for (int i=1;i<Numberg;i++) {
            if((v[i-1]->pos().x()+v[i-1]->width()/2 == v[i]->pos().x()+v[i]->width()/2)&&
                   (v[i-1]->pos().y()<v[i]->pos().y()) )
                winMa++;
            else {
                break;
            }
        }
        //ֻ��ÿ��С�Ķ����ڽ��Ĵ���������ʤ��,winMa���������ж��Ƿ�ÿ��������
        if(winMa == Numberg-1 && *time<= v_time_limit[Numberg-1]){
            wlema = 1;
            if(*time<history::v_time_min[Numberg-1]){   //�Ƚ���ʷ��¼
                rb->show();     //�Ƽ�¼��չʾ�Ƽ�¼
            }
            else {
                vi->show();
            }
            emit end();     //����end�ź��ü�ʱ��Ҳͣ����
        }
        else if (winMa == Numberg-1 && *time> v_time_limit[Numberg-1]) {
            emit end();     //��������ʱ���ֱ�ӽ���
            this->close();
        }
    }
}

MPushButton* Game::find_zhuzi(int x){       //�����ҵ��������������˵�����
    for (int i=0;i<Numberg;i++) {
        if((v[i]->pos().x()+v[i]->width()/2) ==x)
            return v[i];
    }
}

void Game::movezhuzi(int x1, int x2){
    //�洢ִ�в���
    moveQueue.enqueue(qMakePair(x1, x2));
}

void Game::move_hanoi(int Number, int x1, int x2,int x3){
    if(Number==1)
        movezhuzi(x1,x3);
    else {
        move_hanoi(Number-1,x1,x3,x2);
        movezhuzi(x1,x3);
        move_hanoi(Number-1,x2,x1,x3);
    }
}
void Game::startMove(){
    // ����һ���µĶ�ʱ��
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [=](){
        if(!moveQueue.isEmpty()){
            QPair<int, int> move1 = moveQueue.dequeue();
            MPushButton *tmp = find_zhuzi(move1.first);
            tmp->move(move1.second-(tmp->width()/2),0);

            //ʵ������֮���Զ�����
            for(int i=1;i<=Numberg;i++){     //һ��һ���ҿ����ĸ����ӷ����źŵ�,iΪ���ƶ�������

                if(v[i-1]==tmp){
                    int xi = v[i-1]->pos().x();
                    int yi = v[i-1]->pos().y();
                    int width = v[i-1]->width();

                    //jһ��һ����С����ƥ��,��������С������,��ôС�ľ�move���������
                    for (int j=i+1;j<=Numberg+1;j++) {
                        //����ifʵ���Ͼ�ֻ�����ж�����i���������Ѻõ����ӵ�λ���Ƿ���Ͽ���Ҫ��
                        if( j<=Numberg&&
                               yi<=v[j-1]->pos().y()&&
                                   xi-v[j-1]->pos().x()>=-width&&
                                       xi-v[j-1]->pos().x()<=v[j-1]->width() ) {
                            v[i-1]->move(xi,v[j-1]->pos().y()-v[i-1]->height());
                            break;      //ͨ��breakʹ�Լ���Զ�ڴ����Լ���j��Щ����С������
                        }
                        //���i����û�д��,��ô���ƶ���������
                        //ע�������&&����ļ���||����Ҫ������һ��,��Ϊ������һ��&&��ǰ���
                        else if( j == (Numberg+1) &&
                                    ( xi==(581-width/2)||
                                         xi==(260-width/2)||
                                            xi==(900-width/2) )  ) {
                            v[i-1]->move(xi,635);
                            break;
                        }
                    }
                    break;      //breakֱ�Ӳ���ȥ�Һ���İ�ť�ǲ��Ƿ����źŵ�
                }
            }
        } else {
            timer->stop();
            delete timer;
        }
    });
    timer->start(500);
}
//���¿�ʼ��ť��ʵ��
void Game::on_returnButton_2_clicked()
{
    //����һ�й��������¿�ʼ��ʱ
    set_Button();
    step=0;
    moveQueue.clear();
    QString s = QString::number(step);
    ui->label_step->setText("step: "+s);
    time->setHMS(0,0,0);
    timer->start(1000);
    ui->lcdNumber->display(time->toString("hh::mm::ss"));
}

//��ʼ������
void Game::set_Button(){
    for (int i=Numberg;i>=1;i--) {
        v[i-1]->show();
        QString s = QString::number(i);
        v[i-1]->setText(s+s+s);
        v[i-1]->setFixedWidth(100+i*30); //���ð�ť���
        v[i-1]->setFixedHeight(65);      //���ð�ť�߶�
        v[i-1]->setStyleSheet("background-color: rgb( 255, 0, 0);");
        v[i-1]->move(210-i*15,635-(Numberg-i)*65);
        v[i-1]->raise();
    }
}
