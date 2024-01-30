#include "cglobaldetector.h"
#include "ui_cglobaldetector.h"

#include "MyGlobalKeyEvent.h"
#include "MyGlobalMouseEvent.h"

CGlobalDetector::CGlobalDetector(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CGlobalDetector)
{
    ui->setupUi(this);

    connect(CMyGlobalMouseEvent::getInstance(), &CMyGlobalMouseEvent::mouseEvent, this, &CGlobalDetector::on_mouseEvent);
    connect(CMyGlobalMouseEvent::getInstance(), &CMyGlobalMouseEvent::wheelEvent, this, &CGlobalDetector::on_wheelEvent);

    connect(CMyGlobalKeyEvent::getInstance(),&CMyGlobalKeyEvent::keyEvent,this, &CGlobalDetector::on_keyEvent);
}

CGlobalDetector::~CGlobalDetector()
{
    delete ui;
}

void CGlobalDetector::on_mouseEvent(QMouseEvent *event)
{
    switch (event->type())
    {
    case QEvent::MouseButtonPress:   // 鼠标按下
    {
        QString but;
        switch (event->button())
        {
        case Qt::LeftButton:
        {
            but = "左键";
            break;
        }
        case Qt::RightButton:
        {
            but = "右键";
            break;
        }
        case Qt::MiddleButton:
        {
            but = "中键";
            break;
        }
        case Qt::XButton1:
        {
            but = "功能键【后退】";
            break;
        }
        case Qt::XButton2:
        {
            but = "功能键【前进】";
            break;
        }
        default:
        {
            but = "未知";
            break;
        }
        }
        QString str = QString("鼠标%1按下：(x:%2, y:%3)").arg(but).arg(event->x()).arg(event->y());
        ui->textEdit->append(str);
        break;
    }
    case QEvent::MouseMove:     // 鼠标移动
    {
        QString str = QString("鼠标移动：(x:%1, y:%2)").arg(event->x()).arg(event->y());
        ui->textEdit->append(str);
        break;
    }
    case QEvent::MouseButtonRelease:     // 鼠标抬起
    {
        QString but;
        switch (event->button())
        {
        case Qt::LeftButton:
        {
            but = "左键";
            break;
        }
        case Qt::RightButton:
        {
            but = "右键";
            break;
        }
        case Qt::MiddleButton:
        {
            but = "中键";
            break;
        }
        case Qt::XButton1:
        {
            but = "功能键【后退】";
            break;
        }
        case Qt::XButton2:
        {
            but = "功能键【前进】";
            break;
        }
        default:
        {
            but = "未知";
            break;
        }
        }
        QString str = QString("鼠标%1释放：(x:%2, y:%3)").arg(but).arg(event->x()).arg(event->y());
        ui->textEdit->append(str);
        break;
    }
    default:
        break;
    }
    delete event;       // 使用完成后记得delete
}

void CGlobalDetector::on_wheelEvent(QWheelEvent *event)
{
    QString str = QString("鼠标滚轮：%1，(x:%2, y:%3)").arg(event->pixelDelta().x() > 0 ? "向前" : "向后").arg(event->position().x()).arg(event->position().y());
    ui->textEdit->append(str);
    delete event;       // 使用完成后记得delete
}

void CGlobalDetector::on_keyEvent(QKeyEvent *event)
{
    QMetaEnum type = QMetaEnum::fromType<QEvent::Type>();
    QMetaEnum key = QMetaEnum::fromType<Qt::Key>();
    QMetaEnum keyboard = QMetaEnum::fromType<Qt::KeyboardModifiers>();
    QString str = QString("状态：[%1]\t按键：[%2]\t修饰：[%3]]").arg(type.valueToKey(event->type()))
                                           .arg(key.valueToKey(event->key()))
                                           .arg(QString(keyboard.valueToKeys(int(event->modifiers()))));
    if(!event->text().isEmpty())
    {
        str += QString("\t字符：[%1]").arg(event->text());
    }
    ui->textEdit->append(str);
    delete event;       // 使用完成后记得delete
}

//安装全局鼠标监视器
void CGlobalDetector::on_btn_InstallMouse_clicked()
{
    bool ret = CMyGlobalMouseEvent::installMouseEvent();
    ui->textEdit->append(QString("<<<<<<<<<< 全局鼠标事件监听器安装%1 >>>>>>>>>>").arg(ret ? "成功" : "失败"));
}

//卸载全局鼠标监视器
void CGlobalDetector::on_btn_RemoveMouse_clicked()
{
    bool ret = CMyGlobalMouseEvent::removeMouseEvent();
    ui->textEdit->append(QString("<<<<<<<<<< 全局鼠标事件监听器卸载%1 >>>>>>>>>>").arg(ret ? "成功" : "失败"));
}

//安装全局键盘监视器
void CGlobalDetector::on_btn_InstallKey_clicked()
{
    bool ret = CMyGlobalKeyEvent::installKeyEvent();
    ui->textEdit->append(QString("<<<<<<<<<< 全局键盘事件监听器安装%1 >>>>>>>>>>").arg(ret ? "成功" : "失败"));
}

//卸载全局键盘监视器
void CGlobalDetector::on_btn_RemoveKey_clicked()
{
    bool ret = CMyGlobalKeyEvent::removeKeyEvent();
    ui->textEdit->append(QString("<<<<<<<<<< 全局键盘事件监听器卸载%1 >>>>>>>>>>").arg(ret ? "成功" : "失败"));
}

