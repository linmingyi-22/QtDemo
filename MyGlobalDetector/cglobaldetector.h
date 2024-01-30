#ifndef CGLOBALDETECTOR_H
#define CGLOBALDETECTOR_H

#include <QWidget>
#include <QMetaEnum>

QT_BEGIN_NAMESPACE
namespace Ui { class CGlobalDetector; }
QT_END_NAMESPACE

class CGlobalDetector : public QWidget
{
    Q_OBJECT

public:
    CGlobalDetector(QWidget *parent = nullptr);
    ~CGlobalDetector();

private slots:
    //接收信号的槽函数
    void on_mouseEvent(QMouseEvent* event);
    void on_wheelEvent(QWheelEvent* event);
    void on_keyEvent(QKeyEvent* event);

    //按钮槽函数
    void on_btn_InstallMouse_clicked();

    void on_btn_RemoveMouse_clicked();

    void on_btn_InstallKey_clicked();

    void on_btn_RemoveKey_clicked();

private:
    Ui::CGlobalDetector *ui;
};
#endif // CGLOBALDETECTOR_H
