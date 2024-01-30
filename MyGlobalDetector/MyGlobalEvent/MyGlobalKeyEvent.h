#ifndef MYGLOBALKEYEVENT_H
#define MYGLOBALKEYEVENT_H

#include <QObject>
#include <QCursor>
#include <QMouseEvent>

//class QKeyEvent;

/**
 *  全局鼠标事件单例信号类
 */
class CMyGlobalKeyEvent : public QObject
{
    Q_OBJECT
public:
    static CMyGlobalKeyEvent* getInstance()
    {
        static CMyGlobalKeyEvent keyEvent;
        return &keyEvent;
    }

    static bool installKeyEvent();      // 安装全局键盘事件监听器
    static bool removeKeyEvent();       // 卸载全局键盘事件监听器

signals:
    void keyEvent(QKeyEvent* event);

private:
    CMyGlobalKeyEvent(){}
};

#endif // MYGLOBALKEYEVENT_H
