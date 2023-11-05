#ifndef CDEVICESIZETOOL_H
#define CDEVICESIZETOOL_H

#include <QWidget>
#include "Public.h"

enum TabPage
{
    Tab_table,
    Tab_chart
};

QT_BEGIN_NAMESPACE
namespace Ui { class CDeviceSizeTool; }
QT_END_NAMESPACE

class CDeviceSizeTool : public QWidget
{
    Q_OBJECT

public:
    CDeviceSizeTool(QWidget *parent = nullptr);
    ~CDeviceSizeTool();

    void getDeviceInfo();
    void createPieChart(QMap<QString,int> &mapPercentage);
    QColor generateRandomColor();
    void handleSliceHovered(bool state);
private:
    Ui::CDeviceSizeTool *ui;

};
#endif // CDEVICESIZETOOL_H
