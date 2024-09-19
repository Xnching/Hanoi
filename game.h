#ifndef GAME_H
#define GAME_H

#include <QWidget>
#include <dialog.h>
#include <recordbreak.h>
#include <history.h>
#include <victory.h>

#include <QPushButton>
#include <QVector>
#include <QMouseEvent>
#include <QTime>
#include <QTimer>
#include <QLabel>
#include <QQueue>

namespace Ui {              //主游戏界面
class Game;
}


class MPushButton:public QPushButton{
    Q_OBJECT
    friend class Game;
public:

    MPushButton(QWidget *parent = nullptr) : QPushButton(parent) {}
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

signals:
    void ButtonMove();
    void win();
    void start1();
    void wrong1();
private:
    //以p来记录是否点击
    bool p;
    //记录旧位置
    QPoint mouse;
    bool startLeMa=0;

};

class Game : public QWidget
{
    Q_OBJECT

public:
    explicit Game(QWidget *parent = nullptr);
    ~Game();
    void setUi();
    //重写关闭界面事件,此事件会当界面关闭时自动触发
    void closeEvent(QCloseEvent *);
    void movezhuzi(int x1,int x2);
    void move_hanoi(int Number,int x1,int x2,int x3);
    void startMove();
    void zhuzi_kaoqi();
    void set_Button();
    MPushButton* find_zhuzi(int x);
    int showor=0;

signals:
    void back1();
    void end();
    void aboutStepnTime(int NUMBER,int fstep,QTime ftime,bool nwlema);
public slots:
    void set(int NUMBER);

private slots:
    void whether_win();
    void on_returnButton_clicked();
    void ButtonMove_Slot();
    void initTime();
    void updateTime();
    void start_time();
    void end_time();
    void warning1();
    void on_returnButton_2_clicked();

private:

    Dialog *dia = NULL;
    QLabel *time_limit;
    victory *vi = NULL;
    recordBreak *rb =NULL;
    //珠子
    int Numberg;
    MPushButton* button_1;
    MPushButton* button_2;
    MPushButton* button_3;
    MPushButton* button_4;
    MPushButton* button_5;
    MPushButton* button_6;
    QVector<MPushButton*> v;
    QVector<QTime> v_time_limit;
    QQueue<QPair<int, int>> moveQueue;
    QString howTime;
    int step = 0;
    QTimer * timer;
    QTime * time;
    bool wlema=0;
    Ui::Game *ui;
};

#endif // GAME_H
