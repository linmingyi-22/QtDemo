#include "cmyswitch.h"

CMySwitch::CMySwitch(QWidget *parent)
    : QWidget{parent}
{
    isChecked=false;
    buttonStyle=ButtonStyle_init;

    imgOffFile = "";
    imgOnFile = "";
    imgCurFile = imgOffFile;
}

void CMySwitch::mousePressEvent(QMouseEvent *)
{
    imgCurFile = isChecked ? imgOffFile : imgOnFile;
    isChecked = !isChecked;
    this->update();
}


void CMySwitch::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::SmoothPixmapTransform);
    QImage img(imgCurFile);
    img=img.scaled(this->size(),Qt::KeepAspectRatio, Qt::SmoothTransformation);

    //按照比例自动居中绘制
    int pixX = rect().center().x() - img.width() / 2;
    int pixY = rect().center().y() - img.height() / 2;
    QPoint point(pixX, pixY);
    painter.drawImage(point, img);
}

void CMySwitch::setButtonStyle(const ButtonStyle &buttonStyle)
{
    if (this->buttonStyle!=buttonStyle)
    {
        this->buttonStyle=buttonStyle;

        if (buttonStyle==ButtonStyle_1)
        {
            imgOffFile=":/image/btncheckoff1.png";
            imgOnFile=":/image/btncheckon1.png";
            this->resize(87,28);
        }
        else if (buttonStyle==ButtonStyle_2)
        {
            imgOffFile=":/image/btncheckoff2.png";
            imgOnFile=":/image/btncheckon2.png";
            this->resize(87,28);
        }
        else if (buttonStyle==ButtonStyle_3)
        {
            imgOffFile=":/image/btncheckoff3.png";
            imgOnFile=":/image/btncheckon3.png";
            this->resize(96, 38);
        }
        else if (buttonStyle==ButtonStyle_4)
        {
            imgOffFile=":/image/btncheckoff4.png";
            imgOnFile=":/image/btncheckon4.png";
            this->resize(64, 64);
        }

        imgCurFile = isChecked ? imgOnFile : imgOffFile;

        this->update();
        updateGeometry();
    }


}

void CMySwitch::setChecked(bool isChecked)
{
    if (this->isChecked != isChecked)
    {
        this->isChecked = isChecked;
        imgCurFile = isChecked ? imgOnFile : imgOffFile;
        this->update();
    }
}
