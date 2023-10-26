#ifndef BATTERY_H
#define BATTERY_H

#include <QWidget>
#include <QPainter>

class Battery : public QWidget
{
    Q_OBJECT
public:
    explicit Battery(QWidget *parent = nullptr);

    void paintEvent(QPaintEvent *event);
    void drawBorder(QPainter *painter);
    void drawBg(QPainter *painter);
    void drawHead(QPainter *painter);

    void setTargetValue(int value);
    void setTargetValue(double value);

    double getCurrentValue();

    void updateValue();

private:
    double dMinValue;//最小值
    double dMaxValue;//最大值
    double dTargetValue;//目标电量
    double dCurrentValue;//当前电量
    double dDiffValue;//相差电量
    double dAlarmValue;//低电量

    int iBorderWidth;
    int iDiffWidth;//电池体和框的距离

    int iBorderRadius;
    int iBodyRadius;
    int iHeadRadius;//电池头部圆角

    QVector<QColor> vecBodyColorNor;//正常电量时，电池的渐变颜色
    QVector<QColor> vecBodyColorLow;//低电量时，电池的渐变颜色
    QVector<QColor> vecBorderColor;//边框的渐变颜色

    bool bLeft;//是否左移
    QRectF rectBorder;//边框尺寸
    QSize sizeHead;//电池头部的尺寸
    QRectF rectBody;//电池尺寸

    //QTimer timer;
    //int iTimeOutNum;

signals:
    void valueChange(double value);
};

#endif // BATTERY_H
