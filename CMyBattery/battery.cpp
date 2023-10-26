#include "battery.h"
#include <QDebug>

Battery::Battery(QWidget *parent)
    : QWidget{parent}
{
    dMinValue=0.0;
    dMaxValue=100.0;
    dTargetValue=0.0;
    dCurrentValue=0.0;
    dDiffValue=dTargetValue-dCurrentValue;
    dAlarmValue=20;

    iBorderWidth=5;
    iDiffWidth=2;

    iBorderRadius=10;
    iBodyRadius=5;
    iHeadRadius=3;

    //vecBodyColorNor.swap(QVector<QColor>());不正确
    QVector<QColor>().swap(vecBodyColorNor);
    vecBodyColorNor.push_back(QColor(50, 205, 51));
    vecBodyColorNor.push_back(QColor(60, 179, 133));
    QVector<QColor>().swap(vecBodyColorLow);
    vecBodyColorLow.push_back(QColor(250, 118, 113));
    vecBodyColorLow.push_back(QColor(204, 38, 38));
    QVector<QColor>().swap(vecBorderColor);
    vecBorderColor.push_back(QColor(100, 100, 100));
    vecBorderColor.push_back(QColor(80, 80, 80));

    bLeft=false;

    sizeHead.setWidth(20);
    sizeHead.setHeight(50);

    //iTimeOutNum=0;
    //timer.setInterval(10);
    //connect(&timer,&QTimer::timeout,this,&Battery::updateValue);
}

void Battery::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing);

    //绘制边框
    drawBorder(&painter);
    //绘制背景
    drawBg(&painter);
    //绘制头部
    drawHead(&painter);
}

void Battery::drawBorder(QPainter *painter)
{
    painter->save();

    //尺寸不要使用成员变量，要每次都算，避免窗口变小
    QPointF topLeft(0+iBorderRadius,0+iBorderRadius);
    QPointF bottomRight(width()-sizeHead.width(),height()-iBorderRadius);
    rectBorder=QRectF(topLeft,bottomRight);

    painter->setPen(QPen(vecBorderColor[0], iBorderWidth));//创建一个画笔对象，设置其颜色为 borderColorStart，线宽为 borderWidth。
    painter->setBrush(Qt::NoBrush);
    painter->drawRoundedRect(rectBorder, iBorderRadius, iBorderRadius);

//    painter->setPen(QPen(QColor(255,0,0),5));
//    painter->setBrush(Qt::NoBrush);
//    painter->drawPoint(rectBorder.topLeft());
//    painter->drawPoint(rectBorder.topRight());
//    painter->drawPoint(rectBorder.bottomLeft());
//    painter->drawPoint(rectBorder.bottomRight());

    painter->restore();
}

void Battery::drawBg(QPainter *painter)
{
    if(dTargetValue<=dMinValue)
        return;

    painter->save();

    QLinearGradient gradientBody(QPointF(0,0),QPointF(width(), 0));
    QVector<QColor> *vecCor;
    if(dCurrentValue<=dAlarmValue)
    {
        vecCor=&vecBodyColorLow;
    }
    else
    {
        vecCor=&vecBodyColorNor;
    }
    for (int i = 0; i < vecCor->size(); ++i)
    {
        gradientBody.setColorAt((i+1)/vecCor->size(), (*vecCor)[i]);
    }

    QPointF topLeft(rectBorder.topLeft().x()+iDiffWidth, rectBorder.topLeft().y()+iDiffWidth);
    //QPointF bottomRight(rectBorder.bottomRight().x()-iDiffWidth, rectBorder.bottomRight().y()-iDiffWidth);
    double dWidthBody=rectBorder.width()-iDiffWidth*2;
    QPointF bottomRight(rectBorder.topLeft().x()+iDiffWidth+((dCurrentValue/100)*dWidthBody), rectBorder.bottomRight().y()-iDiffWidth);
    emit valueChange(dCurrentValue);

    rectBody=QRectF(topLeft, bottomRight);

    painter->setPen(Qt::NoPen);//创建一个画笔对象，设置其颜色为 borderColorStart，线宽为 borderWidth。
    painter->setBrush(gradientBody);
    painter->drawRoundedRect(rectBody, iBorderRadius, iBorderRadius);

//    painter->setPen(QPen(QColor(255,0,0),5));
//    painter->setBrush(Qt::NoBrush);
//    painter->drawPoint(rectBody.topLeft());
//    painter->drawPoint(rectBody.topRight());
//    painter->drawPoint(rectBody.bottomLeft());
//    painter->drawPoint(rectBody.bottomRight());

    painter->restore();
}

void Battery::drawHead(QPainter *painter)
{
    painter->save();

    QPointF topLeft(width()-sizeHead.width(),(height()/2)-(sizeHead.height()/2));
    QRectF rectHead(topLeft,sizeHead);

    QLinearGradient gradientHead(rectHead.topLeft(), rectHead.bottomLeft());
    for (int i = 0; i < vecBorderColor.size(); ++i)
    {
        gradientHead.setColorAt((i+1)/vecBorderColor.size(), vecBorderColor[i]);
    }

    painter->setPen(Qt::NoPen);
    painter->setBrush(gradientHead);
    painter->drawRoundedRect(rectHead, iHeadRadius, iHeadRadius);

    painter->restore();
}

void Battery::setTargetValue(int value)
{
    setTargetValue(double(value));
}
\

void Battery::setTargetValue(double value)
{
    if(value==dCurrentValue){
        return;
    }

    if(value<dMinValue)
    {
        value=dMinValue;
    }
    else if(value>dMaxValue)
    {
        value=dMaxValue;
    }

    dTargetValue = value;
    dDiffValue=dTargetValue-dCurrentValue;


    updateValue();//更新
}

void Battery::updateValue()
{
    dCurrentValue=dCurrentValue+dDiffValue;
    this->update();

//    使用定时器的话会有延迟现象
//    iTimeOutNum++;
//    if(iTimeOutNum>5)
//    {
//        timer.stop();
//        iTimeOutNum=0;
//    }
//    else
//    {
//        timer.start();
//        dCurrentValue=dCurrentValue+dDiffValue/5;
//        //qDebug()<<"当前电量"<<dCurrentValue;
//        this->update();
//    }

}

double Battery::getCurrentValue()
{
    return this->dCurrentValue;
}
