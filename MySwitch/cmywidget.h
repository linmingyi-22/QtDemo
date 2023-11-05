#ifndef CMYWIDGET_H
#define CMYWIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class CMyWidget; }
QT_END_NAMESPACE

class CMyWidget : public QWidget
{
    Q_OBJECT

public:
    CMyWidget(QWidget *parent = nullptr);
    ~CMyWidget();

    void initForm();
private:
    Ui::CMyWidget *ui;
};
#endif // CMYWIDGET_H
