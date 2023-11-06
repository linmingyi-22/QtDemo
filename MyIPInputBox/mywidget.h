#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include <QtNetwork>

QT_BEGIN_NAMESPACE
namespace Ui { class MyWidget; }
QT_END_NAMESPACE

class MyWidget : public QWidget
{
    Q_OBJECT

public:
    MyWidget(QWidget *parent = nullptr);
    ~MyWidget();

private slots:
    void on_btn_getIP_clicked();

    void on_btn_clear_clicked();

    void getIPAddress();

private:
    Ui::MyWidget *ui;

    QString m_hostName;
    QStringList m_listIP;

    int iNum;//此时是显示第几个ip地址
};
#endif // MYWIDGET_H
