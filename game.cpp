#include "game.h"
#include "ui_game.h"

Game::Game(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Game)
{

    //展示UI
    ui->setupUi(this);
    this->setWindowTitle(QString::fromLocal8Bit("汉诺塔游戏"));
    this->hide();

    //设置计时器
    timer = new QTimer;
    time = new QTime;
    ui->lcdNumber->setDigitCount(8);
    initTime();
    //更新时间的connect与结束的connect在此处,开始的connect由于有button所以在set()里
    connect(timer,SIGNAL(timeout()),this,SLOT(updateTime()));
    connect(this,&Game::end,this,&Game::end_time);

    //存入时间限制
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
    //不能单独写成函数,得要在构造函数里写
    //第三条线位置在x=900,第二条现在x=581,第一条线在x=260,底座顶是y=635
    //将珠子们放到一个容器中,这样可以根据Number来调整各个珠子

    Numberg = NUMBER;
    if(NUMBER<=0||NUMBER>=7)Numberg = 3;

    time_limit = new QLabel(this);
    time_limit->setText(QString::fromLocal8Bit("限时:")+v_time_limit[Numberg-1].toString());
    time_limit->move(15,215);
    //必须要先new再添加不然一直程序异常结束
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

    //开始初始化各个珠子,使其初始时堆在地一根柱子那里
    set_Button();

    //连接信号与槽,要在此处连接,不能在珠子类或其函数内连接,因为Game才是主体
    //当珠子被移动的时候就调用靠齐参函数

    for (int i=0;i<6;i++) {
        connect(v[i],&MPushButton::ButtonMove,this,&Game::ButtonMove_Slot);
        connect(v[i],&MPushButton::wrong1,this,&Game::warning1);
    }

    //有button的connect必须要放在此处
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

void Game::initTime(){          //初始化时间
    time->setHMS(0,0,0);
    ui->lcdNumber->display(time->toString("hh::mm::ss"));
}

void Game::updateTime(){        //更新时间
    *time = time->addSecs(1);
    ui->lcdNumber->display(time->toString("hh::mm::ss"));
}

void Game::start_time(){        //开始计时
    if(showor)return ;
    timer->start(1000);
}

void Game::end_time(){
    if(showor)return ;
    timer->stop();
    howTime  = time->toString();
    emit aboutStepnTime(Numberg,step,*time,wlema);

}

void Game::closeEvent(QCloseEvent *)    //叉掉页面的关闭事件
{
     emit back1();
}

void Game::on_returnButton_clicked()    //返回按钮的实现
{
    emit back1();
    this->close();
}

void MPushButton::mouseMoveEvent(QMouseEvent *event){   //重写移动事件
    //必须先点击自定义控件即珠子,移动事件才会使自定义控件随鼠标移动,而p就是用来判断是否点击的珠子
    if (p) {
        const QPoint delta = event->globalPos() - mouse;
        move(x()+delta.x(), y()+delta.y());
        //更新位置
        mouse = event->globalPos();
    }

    QPushButton::mouseMoveEvent(event);
}

void MPushButton::mousePressEvent(QMouseEvent *event){  //重写点击事件

    //如果左键点击自定义控件,就证明以及开始点击,并记录鼠标位置
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

void MPushButton::mouseReleaseEvent(QMouseEvent *event){    //重写释放事件
    //释放就更改p的状态
    if (event->button() == Qt::LeftButton) {
        p = false;
    }
    emit ButtonMove();
    emit win();
    QPushButton::mouseReleaseEvent(event);
}
//由Press发送,判断是否拖动的非顶端按钮
void Game::warning1(){
    for(int i=0;i<Numberg;i++){
        if(v[i]==sender()){     //找到被拖动的按钮
            int xi = v[i]->pos().x();
            int yi = v[i]->pos().y();
            int width = v[i]->width();
            for (int j=0;j<i;j++) {//从小到大一个一个找,看是否有在被拖得动按钮上面,如果有就没法拖动
                if(v[j]->pos().y()<yi&&
                        xi-v[j]->pos().x()>=-width&&
                            xi-v[j]->pos().x()<=v[j]->width()){
                    dia  = new Dialog();
                    //设置让警告必须被叉掉才能操作游戏界面
                    dia->setWindowModality(Qt::ApplicationModal);
                    dia->show();
                    break;  //不用再往后找了
                }
            }
            break;
        }
    }
}
void Game::ButtonMove_Slot(){       //实现靠齐的槽函数

    this->step++;
    QString s = QString::number(step);
    int i=1;
    //实现自动靠齐柱子
    for (i=1;i<=Numberg;i++){

        int x = v[i-1]->pos().x();
        int y = v[i-1]->pos().y();

        if(v[i-1]==sender()){           //一个一个找看是哪个珠子发出信号的
            if( x >= 661  ){    //一个一个判断离哪个柱子近,离哪个近就移动到哪个柱子那里
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

    //warning2,由Release发送,判断是否是大的珠子要放到小的珠子上
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

    zhuzi_kaoqi();      //实现珠子之间的自动靠齐的函数
    ui->label_step->setText("step: "+s);
}

void Game::zhuzi_kaoqi(){
    //实现珠子之间自动靠齐
    for(int i=1;i<=Numberg;i++){     //一个一个找看是哪个珠子发出信号的,i为被移动的珠子

        if(v[i-1]==sender()){
            int xi = v[i-1]->pos().x();
            int yi = v[i-1]->pos().y();
            int width = v[i-1]->width();

            //j一个一个从小到大匹配,如果大的在小的下面,那么小的就move到大的上面
            for (int j=i+1;j<=Numberg+1;j++) {
                //各种if实际上就只是在判断珠子i相对于下面堆好的珠子的位置是否符合靠齐要求
                if( j<=Numberg&&
                       yi<=v[j-1]->pos().y()&&
                           xi-v[j-1]->pos().x()>=-width&&
                               xi-v[j-1]->pos().x()<=v[j-1]->width() ) {
                    v[i-1]->move(xi,v[j-1]->pos().y()-v[i-1]->height());
                    break;      //通过break使自己永远在大于自己的j那些中最小的上面
                }
                //如果i下面没有大的,那么就移动到底盘上
                //注意这里的&&后面的几个||必须要括号在一起,因为是他们一起&&上前面的
                else if( j == (Numberg+1) &&
                            ( xi==(581-width/2)||
                                 xi==(260-width/2)||
                                    xi==(900-width/2) )  ) {
                    v[i-1]->move(xi,635);
                    break;
                }
            }
            break;      //break直接不用去找后面的按钮是不是发出信号的
        }
    }
}
//实现的判断胜利的槽函数
void Game::whether_win(){
    if(showor)return ;
    int winMa = 0;

    //只需判断最小的珠子是否在右边两根柱子上,然后从最小到下一个,依次比较是否小的堆在大的上面
    if(v[0]->pos().x()>=350){
        for (int i=1;i<Numberg;i++) {
            if((v[i-1]->pos().x()+v[i-1]->width()/2 == v[i]->pos().x()+v[i]->width()/2)&&
                   (v[i-1]->pos().y()<v[i]->pos().y()) )
                winMa++;
            else {
                break;
            }
        }
        //只有每个小的堆在邻近的大的上面才能胜利,winMa就是用来判断是否每个都符合
        if(winMa == Numberg-1 && *time<= v_time_limit[Numberg-1]){
            wlema = 1;
            if(*time<history::v_time_min[Numberg-1]){   //比较历史记录
                rb->show();     //破纪录就展示破纪录
            }
            else {
                vi->show();
            }
            emit end();     //发出end信号让计时器也停下来
        }
        else if (winMa == Numberg-1 && *time> v_time_limit[Numberg-1]) {
            emit end();     //超出限制时间就直接结束
            this->close();
        }
    }
}

MPushButton* Game::find_zhuzi(int x){       //用来找到在这根柱子上最顶端的珠子
    for (int i=0;i<Numberg;i++) {
        if((v[i]->pos().x()+v[i]->width()/2) ==x)
            return v[i];
    }
}

void Game::movezhuzi(int x1, int x2){
    //存储执行操作
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
    // 创建一个新的定时器
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [=](){
        if(!moveQueue.isEmpty()){
            QPair<int, int> move1 = moveQueue.dequeue();
            MPushButton *tmp = find_zhuzi(move1.first);
            tmp->move(move1.second-(tmp->width()/2),0);

            //实现珠子之间自动靠齐
            for(int i=1;i<=Numberg;i++){     //一个一个找看是哪个珠子发出信号的,i为被移动的珠子

                if(v[i-1]==tmp){
                    int xi = v[i-1]->pos().x();
                    int yi = v[i-1]->pos().y();
                    int width = v[i-1]->width();

                    //j一个一个从小到大匹配,如果大的在小的下面,那么小的就move到大的上面
                    for (int j=i+1;j<=Numberg+1;j++) {
                        //各种if实际上就只是在判断珠子i相对于下面堆好的珠子的位置是否符合靠齐要求
                        if( j<=Numberg&&
                               yi<=v[j-1]->pos().y()&&
                                   xi-v[j-1]->pos().x()>=-width&&
                                       xi-v[j-1]->pos().x()<=v[j-1]->width() ) {
                            v[i-1]->move(xi,v[j-1]->pos().y()-v[i-1]->height());
                            break;      //通过break使自己永远在大于自己的j那些中最小的上面
                        }
                        //如果i下面没有大的,那么就移动到底盘上
                        //注意这里的&&后面的几个||必须要括号在一起,因为是他们一起&&上前面的
                        else if( j == (Numberg+1) &&
                                    ( xi==(581-width/2)||
                                         xi==(260-width/2)||
                                            xi==(900-width/2) )  ) {
                            v[i-1]->move(xi,635);
                            break;
                        }
                    }
                    break;      //break直接不用去找后面的按钮是不是发出信号的
                }
            }
        } else {
            timer->stop();
            delete timer;
        }
    });
    timer->start(500);
}
//重新开始按钮的实现
void Game::on_returnButton_2_clicked()
{
    //就是一切归零再重新开始计时
    set_Button();
    step=0;
    moveQueue.clear();
    QString s = QString::number(step);
    ui->label_step->setText("step: "+s);
    time->setHMS(0,0,0);
    timer->start(1000);
    ui->lcdNumber->display(time->toString("hh::mm::ss"));
}

//初始化珠子
void Game::set_Button(){
    for (int i=Numberg;i>=1;i--) {
        v[i-1]->show();
        QString s = QString::number(i);
        v[i-1]->setText(s+s+s);
        v[i-1]->setFixedWidth(100+i*30); //设置按钮宽度
        v[i-1]->setFixedHeight(65);      //设置按钮高度
        v[i-1]->setStyleSheet("background-color: rgb( 255, 0, 0);");
        v[i-1]->move(210-i*15,635-(Numberg-i)*65);
        v[i-1]->raise();
    }
}
