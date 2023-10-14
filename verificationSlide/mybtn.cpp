#include "mybtn.h"

#include <QDebug>
#include <QMouseEvent>

MyBtn::MyBtn(QWidget *parent) : QPushButton(parent) {

    m_inBtn=false;
    m_end=false;

}

void MyBtn::initSldRail(QRect sldRailRect,Widget *widget)
{
    m_SldRailRect=sldRailRect;
    this->geometry();

    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, QColor(0, 0, 0));
    this->setPalette(palette);
    this->setAutoFillBackground(true);

    m_sldRect=this->geometry();
    m_sldRect.setTopLeft(m_SldRailRect.topLeft());//要在设置宽和高之前设置（用moveTopLeft就不需要）
    m_sldRect.setWidth(SldWidget);
    m_sldRect.setHeight(SldHight);
    //sldRect.moveTopLeft(widgetRect.topLeft());
    this->setGeometry(m_sldRect);

    m_widget=widget;
    connect(this,&MyBtn::sldEnd,m_widget,&Widget::sldEnd);
}


void MyBtn::mousePressEvent(QMouseEvent* ev)
{
    if(!m_end)
    {
        m_inBtn=true;
        QPoint curPos=ev->globalPos();//原始点击点
        m_initClick=curPos;
        QPoint sldTopleftPos=m_sldRect.topLeft();
        m_cur2SldX=curPos.x()-sldTopleftPos.x();
    }
}

void MyBtn::mouseMoveEvent(QMouseEvent* ev)
{
    if(ev->globalPos().x()<m_initClick.x())
        return;
    if(!m_end&&m_inBtn)
    {
        m_sldRect.moveTopLeft(QPoint(ev->globalPos().x()-m_cur2SldX,m_sldRect.topLeft().y()));
        this->setGeometry(m_sldRect);

        if(m_sldRect.right()>=m_SldRailRect.right())
        {
            m_end=true;

            emit sldEnd();
        }
    }

}

void MyBtn::mouseReleaseEvent(QMouseEvent* ev)
{
    m_inBtn=false;
    if(!m_end)
    {
        m_sldRect.moveTopLeft(QPoint(m_SldRailRect.x(),m_SldRailRect.y()));
        this->setGeometry(m_sldRect);
    }
}


