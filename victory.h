#ifndef VICTORY_H
#define VICTORY_H

#include <QWidget>

namespace Ui {          //�����Ϸʱ�����ĵ������
class victory;
}

class victory : public QWidget
{
    Q_OBJECT

public:
    explicit victory(QWidget *parent = nullptr);
    ~victory();
private slots:
    void on_pushButton_clicked();

private:
    Ui::victory *ui;
};

#endif // VICTORY_H
