#include "cmytable.h"

CMyTable::CMyTable(QWidget *parent)
    : QTableWidget{parent}
{
    colorBg=QColor(255,255,255);

    vecColorPro.push_back(QColor(100, 184, 255));
    vecColorPro.push_back(QColor(24, 189, 155));
    vecColorPro.push_back(QColor(255, 107, 107));

    vecColorText.push_back(QColor(10, 10, 10));
    vecColorText.push_back(QColor(255, 255, 255));
    vecColorText.push_back(QColor(255, 255, 255));

#if defined(Q_OS_UNIX) && !defined(Q_OS_WASM)
    process = new QProcess(this);
    connect(process, SIGNAL(readyRead()), this, SLOT(readLinuxData()));
#endif

    this->clear();

    //设置列数和列宽
    this->setColumnCount(5);
    this->setColumnWidth(0, 100);
    this->setColumnWidth(1, 120);
    this->setColumnWidth(2, 120);
    this->setColumnWidth(3, 120);
    this->setColumnWidth(4, 120);

    this->setStyleSheet("QTableWidget::item{padding:0px;}");

    QStringList headText;
    headText << "盘符" << "已用空间" << "可用空间" << "总大小" << "已用百分比" ;

    this->setHorizontalHeaderLabels(headText);
    this->setSelectionBehavior(QAbstractItemView::SelectRows);//设置选择行为为选择整行。
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);//禁用编辑触发器。
    this->setSelectionMode(QAbstractItemView::SingleSelection);//设置选择模式为单选模式。不能按Ctrl实现多选
    this->verticalHeader()->setVisible(true);//显示垂直表头。
    this->horizontalHeader()->setStretchLastSection(true);//设置水平表头的最后一列自动伸展。当表格的宽度大于水平表头的总宽度时，最后一列将会自动填充剩余的空间。

    //QMetaObject::invokeMethod(this, "load", Qt::QueuedConnection);
    //QTimer::singleShot(10, this, SLOT(load()));

    //清空原有数据
    int row = this->rowCount();
    for (int i = 0; i < row; ++i) {
        this->removeRow(0);
    }
}

void CMyTable::readLinuxData()
{
#if defined(Q_OS_UNIX) && !defined(Q_OS_WASM)
    while (!process->atEnd()) {
        QString result = QLatin1String(process->readLine());
#ifdef __arm__
        if (result.startsWith("/dev/root")) {
            checkSize(result, "本地存储");
        } else if (result.startsWith("/dev/mmcblk")) {
            checkSize(result, "本地存储");
        } else if (result.startsWith("/dev/mmcblk1p")) {
            checkSize(result, "SD卡");
            QStringList list = result.split(" ");
            emit sdcardReceive(list.at(0));
        } else if (result.startsWith("/dev/sd")) {
            checkSize(result, "U盘");
            QStringList list = result.split(" ");
            emit udiskReceive(list.at(0));
        }
#else
        if (result.startsWith("/dev/sd")) {
            checkSize(result, "");
            QStringList list = result.split(" ");
            emit udiskReceive(list.at(0));
        }
#endif
    }
#endif
}

void CMyTable::insertSize(const QString &name, const QString &use, const QString &free, const QString &total, int percent)
{
    int row=this->rowCount();
    this->insertRow(row);

    QTableWidgetItem *itemName=new QTableWidgetItem(name);
    QTableWidgetItem *itemUse=new QTableWidgetItem(use);
    itemUse->setTextAlignment(Qt::AlignCenter);
    QTableWidgetItem *itemFree=new QTableWidgetItem(free);
    itemFree->setTextAlignment(Qt::AlignCenter);
    QTableWidgetItem *itemTotal=new QTableWidgetItem(total);
    itemTotal->setTextAlignment(Qt::AlignCenter);

    this->setItem(row,0,itemName);
    this->setItem(row,1,itemUse);
    this->setItem(row,2,itemFree);
    this->setItem(row,3,itemTotal);

    QProgressBar *bar=new QProgressBar;
    bar->setRange(0,100);
    bar->setValue(percent);
    bar->setFormat("%p%");//设置进度条格式为百分比形式

    //如果字体一直为白色，当进度太小时，容易和背景色弄在一起

    QString qss=QString("QProgressBar{background-color:%1;border-width:0px;text-align:center;}").arg(colorBg.name());

    if (percent<50) {
        qss+=QString("QProgressBar{color:%1;}QProgressBar::chunk{background-color:%2;}").arg(vecColorText.at(0).name()).arg(LOW50COLOR.name());
    }
    else if (percent<90) {
        qss+=QString("QProgressBar{color:%1;}QProgressBar::chunk{background-color:%2;}").arg(vecColorText.at(1).name()).arg(LOW90COLOR.name());
    }
    else {
        qss+=QString("QProgressBar{color:%1;}QProgressBar::chunk{background-color:%2;}").arg(vecColorText.at(2).name()).arg(NORCOLOR.name());
    }

    bar->setStyleSheet(qss);
    this->setCellWidget(row, 4, bar);

}
