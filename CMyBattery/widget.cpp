#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    //tab widget初始化
    ui->tabWidget->setCurrentIndex(Opt_Hand);


    //LCD Number初始化
    connect(ui->widget,SIGNAL(valueChange(double)),this,SLOT(setLCDNumber(double)));

    //计时器初始化
    timer.setInterval(100);
    connect(&timer,&QTimer::timeout,this,&Widget::charge);
}

Widget::~Widget()
{
    delete ui;
}

//LCD Number相关操作
void Widget::setLCDNumber(double number)
{
    ui->lcd_number->display(QString::number(number,'f',1));
}


int Widget::getLCDNumber()
{
    return QString::number(ui->lcd_number->value()).replace(LCDNumberUnit, "").toInt();;
}

//滑动块变化
void Widget::on_hSlider_valueChanged(int value)
{
    ui->widget->setTargetValue(value*0.1);

    ui->doubleSpinBox->setValue(value*0.1);
}

//设置电量
void Widget::on_pushButton_clicked()
{
    int number=ui->doubleSpinBox->value();
    ui->widget->setTargetValue(number);

    ui->hSlider->setValue(number*10);
}

//
void Widget::charge()
{
    double currentValue=ui->widget->getCurrentValue();
    if(currentValue==100)
    {
        timer.stop();
    }
    else
    {
        ui->widget->setTargetValue(currentValue+0.1);

        ui->hSlider->setValue((currentValue+0.1)*10);
    }
}

//开始充电
void Widget::on_btn_start_clicked()
{
    double currentValue=ui->widget->getCurrentValue();
    if(currentValue==100)
    {
        QMessageBox::information(this,"提示","已经充满了");
        return;
    }
    else
    {
        if(timer.isActive())
        {
            QMessageBox::information(this,"提示","已经正在充电");

        }
        else
        {
            timer.start();
        }
    }
}

//停止充电
void Widget::on_btn_stop_clicked()
{
    timer.stop();
}

