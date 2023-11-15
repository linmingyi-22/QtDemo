#ifndef CMYWIDGET_H
#define CMYWIDGET_H

#include <QWidget>
#include "iconhelper.h"
#include "mynavbutton.h"

QT_BEGIN_NAMESPACE
namespace Ui { class CMyWidget; }
QT_END_NAMESPACE

class CMyWidget : public QWidget
{
    Q_OBJECT

public:
    CMyWidget(QWidget *parent = nullptr);
    ~CMyWidget();

private:
    Ui::CMyWidget *ui;

    QList<int> m_listIcons;//图标

    QList<CMyNavButton *> btns1;
    QList<CMyNavButton *> btns2;
    QList<CMyNavButton *> btns3;
    QList<CMyNavButton *> btns4;
    QList<CMyNavButton *> btns5;
    QList<CMyNavButton *> btns6;
    QList<CMyNavButton *> btns7;

private slots:
    void initForm();
    void initBtn1();
    void initBtn2();
    void initBtn3();
    void initBtn4();
    void initBtn5();
    void initBtn6();
    void initBtn7();

    void buttonClick1();
    void buttonClick2();
    void buttonClick3();
    void buttonClick4();
    void buttonClick5();
    void buttonClick6();
    void buttonClick7();
};
#endif // CMYWIDGET_H
