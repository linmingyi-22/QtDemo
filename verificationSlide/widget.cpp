#include "widget.h"
#include "ui_widget.h"

#include <QPalette>
#include <QColor>
#include <QPushButton>
#include <QMouseEvent>
#include <QDebug>
#include <QMessageBox>
#include <QThread>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    //初始化
    m_mousePress=false;
    m_init=0;

    //设置背景颜色
    QPalette palette = ui->sldRail->palette();
    palette.setColor(QPalette::Window, QColor(255, 255, 255));
    ui->sldRail->setPalette(palette);
    ui->sldRail->setAutoFillBackground(true);//启用控件的自动填充背景功能(使QPalette中设置的背景颜色生效)


    QRect windowRect=this->geometry();
    QPoint windowPoint=windowRect.center();

    m_sldRailRect=ui->sldRail->geometry();
    m_sldRailRect.setWidth(SldRailLength);
    m_sldRailRect.setHeight(SldHight);
    m_sldRailRect.moveCenter(windowPoint);
    ui->sldRail->setGeometry(m_sldRailRect);

    m_timerTemp.setSingleShot(true);
    m_timerTemp.setInterval(10);
    connect(&m_timerTemp,&QTimer::timeout,this,&Widget::initBtn);
    m_timerTemp.start();

    m_point[0]=ui->point1;
    m_point[1]=ui->point2;
    m_point[2]=ui->point3;
    m_pointPix[0].load(":/image/arrow_grey.png");
    m_pointPix[1].load(":/image/arrow_green.png");
    m_pointPix[2].load(":/image/right.png");


    QRect rectRight=ui->pointRight->geometry();
    rectRight.setWidth(SldWidget);
    rectRight.setHeight(SldHight);
    rectRight.moveCenter(m_sldRailRect.center());
    ui->pointRight->setGeometry(rectRight);
    ui->pointRight->setPixmap(m_pointPix[2]);
    ui->pointRight->setAlignment(Qt::AlignCenter);//居中
    ui->pointRight->setScaledContents(true);//自动缩放图以适应控件大小
    ui->pointRight->hide();

    for(int i=0;i<3;i++)
    {
        m_pointRect[i].setWidth(SldWidget);
        m_pointRect[i].setHeight(SldHight);
        m_pointRect[i].moveTopLeft(QPoint(m_sldRailRect.x()+(SldWidget+SldWidget)*(i+1),m_sldRailRect.y()));
        m_point[i]->setGeometry(m_pointRect[i]);
        m_point[i]->setPixmap(m_pointPix[0]);
        m_point[i]->setAlignment(Qt::AlignCenter);//居中
        m_point[i]->setScaledContents(true);//自动缩放图以适应控件大小

        m_timer[i].setInterval(1000+(i)*50);
        m_mapTimerID.insert(std::make_pair(&m_timer[i],i));
        m_mapPixIndex.insert(std::make_pair(&m_timer[i],0));
        connect(&m_timer[i],&QTimer::timeout,this,&Widget::pointChange);
        m_timer[i].start();
    }
}

Widget::~Widget()
{
    delete ui;
}

void Widget::sldEnd()
{
    QMessageBox::information(this,"提示","已经到达终点");

    for(int i=0;i<3;i++)
    {
        m_point[i]->hide();
        m_timer[i].stop();
    }

    ui->pointRight->show();
}

void Widget::initBtn()
{
    ui->btnSld->initSldRail(m_sldRailRect,this);//传入导轨信息
}

void Widget::pointChange()
{
    QTimer* senderTimer = qobject_cast<QTimer*>(sender());
    if (senderTimer)
    {
        int timerId = m_mapTimerID[senderTimer]; // 获取计时器的ID
        int pixIndex=m_mapPixIndex[senderTimer];
        if(pixIndex==0)
            m_mapPixIndex[senderTimer]=1;
        else
            m_mapPixIndex[senderTimer]=0;
        m_point[timerId]->setPixmap(m_pointPix[pixIndex]);
        m_point[timerId]->setAlignment(Qt::AlignCenter);//居中
        m_point[timerId]->setScaledContents(true);//自动缩放图以适应控件大小
    }

}
