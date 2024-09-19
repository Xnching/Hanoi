#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {          //用来展示警告，例如游戏途中出现违规操作
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
