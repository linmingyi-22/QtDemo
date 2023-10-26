#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTimer>
#include <QMessageBox>

#define LCDNumberUnit "%"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

enum TabPage
{
  Opt_Hand,
  Opt_Auto,
  Opt_Set
};


class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    //LCD Number控件设置
    int getLCDNumber();

    QTimer timer;

private slots:
    void on_hSlider_valueChanged(int value);
    void setLCDNumber(double number);
    void charge();

    void on_pushButton_clicked();

    void on_btn_start_clicked();

    void on_btn_stop_clicked();

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
