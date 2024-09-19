#ifndef BREAK_H
#define BREAK_H

#include <QWidget>

namespace Ui {
class break1;
}

class break1 : public QWidget
{
    Q_OBJECT

public:
    explicit break1(QWidget *parent = nullptr);
    ~break1();

private:
    Ui::break1 *ui;
};

#endif // BREAK_H
