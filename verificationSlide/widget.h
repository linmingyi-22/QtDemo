#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTimer>
#include <QLabel>
#include <map>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE


#define SldHight 50//滑块高度
#define SldWidget 50//滑块宽度
#define SldRailLength 500//滑轨长度

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;

    //控件的位置
    QRect m_sldRailRect;
    QRect m_sldRect;
    QRect m_pointRect[3];
    QLabel *m_point[3];
    QTimer m_timer[3];
    std::map<QTimer*,int> m_mapTimerID;
    std::map<QTimer*,int> m_mapPixIndex;
    QPixmap m_pointPix[3];

    bool m_mousePress;//是否按下
    QPoint m_mousePos;//按下时按钮位置

    int m_init;
    QTimer m_timerTemp;
public slots:
    void sldEnd();
    void pointChange();
    void initBtn();
};
#endif // WIDGET_H
