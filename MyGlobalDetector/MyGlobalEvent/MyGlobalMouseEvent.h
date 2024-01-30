#ifndef MYGLOBALMOUSEEVENT_H
#define MYGLOBALMOUSEEVENT_H

#include <QObject>
#include <QMouseEvent>
#include <QCursor>

//class QMouseEvent;
//class QWheelEvent;

class CMyGlobalMouseEvent : public QObject
{
    Q_OBJECT
public:
    static CMyGlobalMouseEvent* getInstance()
    {
        static CMyGlobalMouseEvent mouseEvent;
        return &mouseEvent;
    }

    static bool installMouseEvent();      // 安装全局鼠标事件监听器
    static bool removeMouseEvent();       // 卸载全局鼠标事件监听器

signals:
    void mouseEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent* event);

private:
    CMyGlobalMouseEvent(){}

};

#endif // MYGLOBALMOUSEEVENT_H
