#ifndef EXPLAIN_H
#define EXPLAIN_H

#include <QWidget>

namespace Ui {      //��Ϊ��Ϸ˵��
class explain;
}

class explain : public QWidget
{
    Q_OBJECT

public:
    explicit explain(QWidget *parent = nullptr);
    ~explain();

    void closeEvent(QCloseEvent *);
signals:
    void back();
private slots:
    void on_pushButton_clicked();

private:
    Ui::explain *ui;



};

#endif // EXPLAIN_H
