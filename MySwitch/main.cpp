#include "cmywidget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CMyWidget w;
    w.show();
    return a.exec();
}
