#ifndef CMYSWITCH_H
#define CMYSWITCH_H

#include <QWidget>
#include <QPainter>

enum ButtonStyle {
    ButtonStyle_init=-1,
    ButtonStyle_1,  //开关样式1
    ButtonStyle_2,  //开关样式2
    ButtonStyle_3,  //开关样式3
    ButtonStyle_4   //开关样式4
};

class CMySwitch : public QWidget
{
    Q_OBJECT
public:
    explicit CMySwitch(QWidget *parent = nullptr);

    //获取和设置按钮样式
    void setButtonStyle(const ButtonStyle &buttonStyle);

    //设置是否选中
    void setChecked(bool isChecked);
protected:
    void mousePressEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *event);

private:
    bool isChecked;//是否选中
    ButtonStyle buttonStyle;//按钮样式

    QString imgOffFile;//关闭图片
    QString imgOnFile;//开启图片
    QString imgCurFile;//当前图片

signals:

};

#endif // CMYSWITCH_H
