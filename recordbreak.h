#ifndef RECORDBREAK_H
#define RECORDBREAK_H

#include <QWidget>

namespace Ui {              //�Ƽ�¼�����
class recordBreak;
}

class recordBreak : public QWidget
{
    Q_OBJECT

public:
    explicit recordBreak(QWidget *parent = nullptr);
    ~recordBreak();
private slots:
    void on_pushButton_clicked();

private:
    Ui::recordBreak *ui;
};

#endif // RECORDBREAK_H
