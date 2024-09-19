#ifndef NUMBER_H
#define NUMBER_H

#include <QWidget>
#include <game.h>
#include <show.h>
namespace Ui {              //此为输入想要游玩多少珠子的弹窗
class number;
}

class number : public QWidget
{
    Q_OBJECT

public:
    explicit number(QWidget *parent = nullptr);
    ~number();
    int Number;
    Game *g1=NULL;
    void closeEvent(QCloseEvent *);
    bool showorgame = 0;
signals:
    void back();
    void numberEntered(int NUMBER);
private slots:
    void on_pushButton_clicked();
    void save();
private:
    Ui::number *ui;
};

#endif // NUMBER_H
