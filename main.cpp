#include "widget.h"
#include <QApplication>


//�����ļ������ж�����Ҫ�ڹ��캯����ʵ��
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Widget *w=new Widget();
    w->show();

    return a.exec();
}
