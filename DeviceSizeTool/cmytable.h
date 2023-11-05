#ifndef CMYTABLE_H
#define CMYTABLE_H

#include <QTableWidget>
#include <QHeaderView>
#include <QProcess>
#include <QDir>
#include <QProgressBar>
#include "Public.h"

class CMyTable : public QTableWidget
{
    Q_OBJECT
public:
    explicit CMyTable(QWidget *parent = nullptr);

private:
    QProcess *process;//执行命令的进程

    QColor colorBg;
    QVector<QColor> vecColorPro;//进度颜色
    QVector<QColor> vecColorText;//文字颜色
signals:

public slots:
    void insertSize(const QString &name, const QString &use, const QString &free, const QString &all, int percent);


private slots:
    void readLinuxData();


};

#endif // CMYTABLE_H
