#include "widget.h"
#include <QApplication>


//启动文件，所有东西主要在构造函数中实现
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Widget *w=new Widget();
    w->show();

    return a.exec();
}
