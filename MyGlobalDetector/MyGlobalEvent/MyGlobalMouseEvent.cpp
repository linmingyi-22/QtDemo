#include "MyGlobalMouseEvent.h"

#if defined(Q_OS_WIN)

#include "Windows.h"

static HHOOK g_hook = nullptr;//用于避免重复安装

//信号接收函数
LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    QPoint point = QCursor::pos();  // 获取鼠标当前位置
    switch (wParam)
    {
    case WM_LBUTTONDOWN:   // 鼠标左键按下
        emit CMyGlobalMouseEvent::getInstance()->mouseEvent(new QMouseEvent(QEvent::MouseButtonPress, point, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier));
        break;
    case WM_MOUSEMOVE:     // 鼠标移动
        emit CMyGlobalMouseEvent::getInstance()->mouseEvent(new QMouseEvent(QEvent::MouseMove, point, Qt::NoButton, Qt::NoButton, Qt::NoModifier));
        break;
    case WM_RBUTTONDOWN:   // 鼠标右键按下
        emit CMyGlobalMouseEvent::getInstance()->mouseEvent(new QMouseEvent(QEvent::MouseButtonPress, point, Qt::RightButton, Qt::RightButton, Qt::NoModifier));
        break;
    case WM_RBUTTONUP:     // 鼠标右键抬起
        emit CMyGlobalMouseEvent::getInstance()->mouseEvent(new QMouseEvent(QEvent::MouseButtonRelease, point, Qt::RightButton, Qt::RightButton, Qt::NoModifier));
        break;
    case WM_LBUTTONUP:     // 鼠标左键抬起
        emit CMyGlobalMouseEvent::getInstance()->mouseEvent(new QMouseEvent(QEvent::MouseButtonRelease, point, Qt::LeftButton, Qt::LeftButton, Qt::NoModifier));
        break;
    case WM_MOUSEWHEEL:    // 鼠标滚轮
    {
        MSLLHOOKSTRUCT * msll = reinterpret_cast<MSLLHOOKSTRUCT*>(lParam);

        int pixelOffset = GET_WHEEL_DELTA_WPARAM(msll->mouseData) / WHEEL_DELTA;
        //GET_WHEEL_DELTA_WPARAM用于提取滚动距离，单位为像素，除以WHEEL_DELTA，可获得标准化的滚动距离
        int degreeOffset = (GET_KEYSTATE_WPARAM(msll->mouseData) >> 16) / WHEEL_DELTA;
        //GET_KEYSTATE_WPARAM用于提取滚动状态信息（他的高位直接表示度数偏移量），除以WHEEL_DELTA，可获得标准化的滚动度数

        emit CMyGlobalMouseEvent::getInstance()->wheelEvent(new QWheelEvent(point,QPoint(msll->pt.x, msll->pt.y), QPoint(pixelOffset, 0),
                                                                         QPoint(degreeOffset, 0), Qt::MiddleButton, Qt::NoModifier,Qt::ScrollUpdate,false));
        break;
    }
    case WM_MBUTTONUP:     // 鼠标中键释放
        emit CMyGlobalMouseEvent::getInstance()->mouseEvent(new QMouseEvent(QEvent::MouseButtonRelease, point, Qt::MiddleButton, Qt::MiddleButton, Qt::NoModifier));
        break;
    case WM_MBUTTONDOWN:   // 鼠标中键按下
        emit CMyGlobalMouseEvent::getInstance()->mouseEvent(new QMouseEvent(QEvent::MouseButtonPress, point, Qt::MiddleButton, Qt::MiddleButton, Qt::NoModifier));
        break;
    case WM_XBUTTONDOWN:   // 鼠标功能键按下
    {
        MSLLHOOKSTRUCT * msll = reinterpret_cast<MSLLHOOKSTRUCT*>(lParam);
        int Button = GET_XBUTTON_WPARAM (msll->mouseData);
        switch (Button)
        {
        case XBUTTON1:   // 后退
            emit CMyGlobalMouseEvent::getInstance()->mouseEvent(new QMouseEvent(QEvent::MouseButtonPress, point, Qt::XButton1, Qt::XButton1, Qt::NoModifier));
            break;
        case XBUTTON2:   // 前进
            emit CMyGlobalMouseEvent::getInstance()->mouseEvent(new QMouseEvent(QEvent::MouseButtonPress, point, Qt::XButton2, Qt::XButton2, Qt::NoModifier));
            break;
        }
        break;
    }
    case WM_XBUTTONUP:   // 鼠标功能键释放
    {
        MSLLHOOKSTRUCT * msll = reinterpret_cast<MSLLHOOKSTRUCT*>(lParam);
        int Button = GET_XBUTTON_WPARAM (msll->mouseData);
        switch (Button)
        {
        case XBUTTON1:
            emit CMyGlobalMouseEvent::getInstance()->mouseEvent(new QMouseEvent(QEvent::MouseButtonRelease, point, Qt::XButton1, Qt::XButton1, Qt::NoModifier));
            break;
        case XBUTTON2:
            emit CMyGlobalMouseEvent::getInstance()->mouseEvent(new QMouseEvent(QEvent::MouseButtonRelease, point, Qt::XButton2, Qt::XButton2, Qt::NoModifier));
            break;
        }
        break;
    }
    default:
        break;
    }

    return CallNextHookEx(nullptr, nCode, wParam, lParam);//将消息传递给下一个挂钩过程，并返回其返回值。
}


//安装全局鼠标监听器
bool CMyGlobalMouseEvent::installMouseEvent()
{
    if(g_hook!=nullptr)
        return true;
    g_hook=SetWindowsHookExW(WH_MOUSE_LL,LowLevelMouseProc,GetModuleHandleW(nullptr),0);
    return g_hook != nullptr ? true:false;
}

bool CMyGlobalMouseEvent::removeMouseEvent()
{
    if(!g_hook)
        return true;
    bool ret = UnhookWindowsHookEx(g_hook);
    if(ret)
    {
        g_hook = nullptr;
        return true;
    }
    return false;
}

#endif
