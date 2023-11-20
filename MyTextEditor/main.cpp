#include "mytexteditor.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CMyTextEditor w;
    w.show();
    return a.exec();
}
