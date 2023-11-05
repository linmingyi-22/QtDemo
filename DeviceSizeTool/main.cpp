#include "cdevicesizetool.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CDeviceSizeTool w;
    w.show();
    srand(time(NULL));
    return a.exec();
}
