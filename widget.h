#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <explain.h>
#include <number.h>
#include <game.h>
#include <history.h>

namespace Ui {              //游戏开始界面
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:

    void on_ExitButton_clicked();
    void on_ShowButton_clicked();
    void on_HistoryButton_clicked();
    void on_ExplainButton_clicked();
    void on_StartButton_clicked();

private:
    Ui::Widget *ui;
    explain *e = NULL;
    number *n = NULL;
    history *h = NULL;
    victory *vm;
};

#endif // WIDGET_H
