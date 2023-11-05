#include "cdevicesizetool.h"
#include "ui_cdevicesizetool.h"

#include <QPieSeries>
#include <QPieSlice>
#include <QChart>
#include <QChartView>

#ifdef Q_OS_WIN
#include "windows.h"
#endif

CDeviceSizeTool::CDeviceSizeTool(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CDeviceSizeTool)
{
    ui->setupUi(this);

    ui->tabWidget->setCurrentIndex(Tab_table);

    getDeviceInfo();

    //ui->widget->setFixedSize(QSize(800,800));
}

CDeviceSizeTool::~CDeviceSizeTool()
{
    delete ui;
}

void CDeviceSizeTool::getDeviceInfo()
{
#ifdef Q_OS_WIN
    QFileInfoList list = QDir::drives();

    QMap<QString,int> mapPercentage;
    foreach (QFileInfo dir, list) {
        QString dirName = dir.absolutePath();
        LPCWSTR lpcwstrDriver = (LPCWSTR)dirName.utf16();//将dirName转换为宽字符类型（LPCWSTR）的指针。这是为了后续调用Windows API函数而进行的类型转换。
        ULARGE_INTEGER liFreeBytesAvailable, liTotalBytes, liTotalFreeBytes;

        if (GetDiskFreeSpaceEx(lpcwstrDriver, &liFreeBytesAvailable, &liTotalBytes, &liTotalFreeBytes)) {
            QString use = QString::number((double)(liTotalBytes.QuadPart - liTotalFreeBytes.QuadPart) / GB, 'f', 1);
            use += "G";
            QString free = QString::number((double) liTotalFreeBytes.QuadPart / GB, 'f', 1);
            free += "G";
            QString total = QString::number((double) liTotalBytes.QuadPart / GB, 'f', 1);
            total += "G";
            int percent = 100 - ((double)liTotalFreeBytes.QuadPart / liTotalBytes.QuadPart) * 100;

            ui->tableWidget->insertSize(dirName, use, free, total, percent);
            mapPercentage.insert(dirName,percent);
        }
    }
    createPieChart(mapPercentage);

#elif defined(Q_OS_UNIX) && !defined(Q_OS_WASM)
    process->start("df", QStringList() << "-h");
#endif

}

void CDeviceSizeTool::createPieChart(QMap<QString,int> &mapPercentage)
{
    // 创建一个饼图数据系列
    QPieSeries *series = new QPieSeries();

    for (auto it = mapPercentage.begin(); it != mapPercentage.end(); ++it) {
        // 创建每个饼图分片
        QPieSlice *slice = new QPieSlice(QString("%1").arg(it.key()), it.value());
        slice->setLabelVisible();

        // 生成随机颜色
        QColor randomColor = generateRandomColor();
        slice->setBrush(randomColor);

        // 添加分片到系列
        series->append(slice);

        // 连接鼠标移入事件的信号与槽
        connect(slice, &QPieSlice::hovered, this, &CDeviceSizeTool::handleSliceHovered);
    }

    // 创建一个饼图
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("磁盘使用空间的占比情况");

    // 创建一个视图并设置饼图
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // 将图表视图添加到传入的 widget 中
    chartView->setParent(ui->widget);
    chartView->resize(ui->widget->size());
    qDebug()<<ui->widget->size();
}

QColor CDeviceSizeTool::generateRandomColor()
{
    // 生成随机颜色
    int red = rand() % 256;
    int green = rand() % 256;
    int blue = rand() % 256;
    return QColor(red, green, blue);
}

void CDeviceSizeTool::handleSliceHovered(bool state)
{
    QPieSlice *slice = qobject_cast<QPieSlice*>(sender());

    if (slice) {
        if (state) {
            // 鼠标移入扇形，使其突出
            slice->setExploded(true);
        } else {
            // 鼠标移出扇形，取消突出显示
            slice->setExploded(false);
        }
    }
}

