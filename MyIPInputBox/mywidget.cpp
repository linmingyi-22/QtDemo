#include "mywidget.h"
#include "ui_mywidget.h"

MyWidget::MyWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MyWidget)
{
    ui->setupUi(this);

    m_hostName=QHostInfo::localHostName();
    getIPAddress();
    iNum=-1;
}

MyWidget::~MyWidget()
{
    delete ui;
}

void MyWidget::getIPAddress()
{
    QHostInfo info = QHostInfo::fromName(m_hostName);
    m_listIP.clear();
    foreach(QHostAddress address,info.addresses())
    {
        if(address.protocol() == QAbstractSocket::IPv4Protocol)
        {
            m_listIP.append(address.toString());
        }

    }
}

//显示IP
void MyWidget::on_btn_getIP_clicked()
{
    iNum= (iNum+1)%m_listIP.count();

    ui->widget->setIPAddress(m_listIP.at(iNum));

    QString strTarget=QString("获取IP (%1/%2)").arg(iNum+1).arg(m_listIP.count());
    ui->btn_getIP->setText(strTarget);
}

//清空
void MyWidget::on_btn_clear_clicked()
{
    ui->widget->clear();
    ui->btn_getIP->setText("获取IP");
}

