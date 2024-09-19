#ifndef HISTORY_H
#define HISTORY_H

#include <QWidget>
#include <QTime>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QDir>
#include <QStringList>
namespace Ui {          //记录历史纪录并存储在文件中
class history;
}

class history : public QWidget
{
    Q_OBJECT
    friend class Game;
public:
    explicit history(QWidget *parent = nullptr);
    ~history();
    void closeEvent(QCloseEvent *);

    static std::vector<QTime> v_time_min ;

    static std::vector<int> v_step_min ;
    QFile qfs;
    QStringList linelist;
    void set();

public slots:
    void game_step_time(int Number,int gstep,QTime time,bool wlema=0);

signals:
    void back3();
private slots:
    void on_pushButton_clicked();
private:
    QString data;
    Ui::history *ui;
};

#endif // HISTORY_H
