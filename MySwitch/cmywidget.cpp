#include "cmywidget.h"
#include "ui_cmywidget.h"

CMyWidget::CMyWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CMyWidget)
{
    ui->setupUi(this);
    this->initForm();
}

CMyWidget::~CMyWidget()
{
    delete ui;
}


void CMyWidget::initForm()
{
    ui->switch1->setFixedSize(87, 30);
    ui->switch2->setFixedSize(87, 30);
    ui->switch3->setFixedSize(87, 30);
    ui->switch4->setFixedSize(64, 64);

    ui->switch1->setButtonStyle(ButtonStyle::ButtonStyle_1);
    ui->switch2->setButtonStyle(ButtonStyle::ButtonStyle_2);
    ui->switch3->setButtonStyle(ButtonStyle::ButtonStyle_3);
    ui->switch4->setButtonStyle(ButtonStyle::ButtonStyle_4);

    ui->switch1->setChecked(true);
    ui->switch2->setChecked(true);
    ui->switch3->setChecked(true);
    ui->switch4->setChecked(true);
}
