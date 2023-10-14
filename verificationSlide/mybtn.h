#ifndef MYBTN_H
#define MYBTN_H

#include <QWidget>
#include <QPushButton>
#include <widget.h>

#define SldHight 50//滑块高度
#define SldWidget 50//滑块宽度

class MyBtn : public QPushButton
{
    Q_OBJECT
public:
    explicit MyBtn(QWidget *parent = nullptr);

    void initSldRail(QRect sldRailRect,Widget *widget);


    void mousePressEvent(QMouseEvent* ev);
    void mouseReleaseEvent(QMouseEvent* ev);
    void mouseMoveEvent(QMouseEvent* ev);

private:
    QRect m_SldRailRect;
    QRect m_sldRect;
    int m_cur2SldX;
    QPoint m_initClick;

    Widget *m_widget;
    bool m_end;//是否已经到达终点
    bool m_inBtn;//是否点击了按钮

signals:
    void sldEnd();

};

#endif // MYBTN_H
