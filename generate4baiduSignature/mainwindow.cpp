#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <time.h>
#include <string>
#include <iostream>
using namespace std;
#include <QMessageBox>

#include <QCryptographicHash>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle(QString::fromLocal8Bit("百度翻译APIsign生成器"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

//生成随机数
void MainWindow::on_btn_genSalt_clicked()
{
    ui->le_salt->clear();

    srand(time(NULL));

    int number=ui->spinBox->value();
    string saltTemp="";
    for (int i = 0; i < number; ++i) {
        saltTemp += '0' + (rand() % 10);
    }

    QString salt = QString::fromStdString(saltTemp);
    ui->le_salt->setText(salt);
}


void MainWindow::on_btn_genSign_clicked()
{
    if (ui->le_appid->text().isEmpty()) {
        QMessageBox::warning(this, "提示", "appid不能为空");
        return;
    }
    else if (ui->le_query->text().isEmpty()) {
        QMessageBox::warning(this, "提示", "翻译内容不能为空");
        return;
    }
    else if (ui->le_salt->text().isEmpty()) {
        on_btn_genSalt_clicked();
    }
    else if (ui->le_pwd->text().isEmpty()) {
        QMessageBox::warning(this, "提示", "密钥不能为空");
        return;
    }

    QString apiKey=ui->le_appid->text();
    QString query=ui->le_query->text();
    QString salt=ui->le_salt->text();
    QString secretKey=ui->le_pwd->text();

    //核心
    // 构建签名字符串
    QString signKey = apiKey + query + salt + secretKey;

    QByteArray inputData = signKey.toUtf8();
    QByteArray md5Hash = QCryptographicHash::hash(inputData, QCryptographicHash::Md5);

    ui->le_sign->setText(QString(md5Hash.toHex()));
}

