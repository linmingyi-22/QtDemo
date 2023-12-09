#include "myinputbox.h"

MyInputBox::MyInputBox(QWidget *parent)
    : QWidget{parent}
{
    bgColor="#FFFFFF";
    borderColor="#000000";
    borderRadius=3;

    for (int i = 0; i < 3; ++i)
    {
        QLabel *lblTemp=new QLabel;
        lblTemp->setAlignment(Qt::AlignCenter);
        lblTemp->setText(".");

        m_vecPoint.push_back(lblTemp);
    }

    for (int i = 0; i < 4; ++i)
    {
        QLineEdit *lblIP=new QLineEdit;
        lblIP->setAlignment(Qt::AlignCenter);
        lblIP->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        connect(lblIP, SIGNAL(textChanged(QString)), this, SLOT(textChanged(QString)));

        //设置IP地址校验过滤
        QString pattern = "(2[0-5]{2}|2[0-4][0-9]|1?[0-9]{1,2})";
        //确切的说 QRegularExpression QRegularExpressionValidator 从5.0 5.1开始就有
    #if (QT_VERSION >= QT_VERSION_CHECK(6,0,0))
        QRegularExpression regExp(pattern);
        QRegularExpressionValidator *validator = new QRegularExpressionValidator(regExp, this);
    #else
        QRegExp regExp(pattern);
        QRegExpValidator *validator = new QRegExpValidator(regExp, lblIP);
    #endif
        lblIP->setValidator(validator);

        lblIP->installEventFilter(this);

        m_vecIP.push_back(lblIP);
    }

    QFrame *frame = new QFrame;
    frame->setObjectName("frameIP");

    QStringList qss;
    qss.append(QString("QFrame#frameIP{border:1px solid %1;border-radius:%2px;}").arg(borderColor.name()).arg(borderRadius));
    qss.append(QString("QLabel{min-width:15px;background-color:%1;}").arg(bgColor.name()));
    qss.append(QString("QLineEdit{background-color:%1;border:none;}").arg(bgColor.name()));
    qss.append(QString("QLineEdit#txtIP1{border-top-left-radius:%1px;border-bottom-left-radius:%1px;}").arg(borderRadius));
    qss.append(QString("QLineEdit#txtIP4{border-top-right-radius:%1px;border-bottom-right-radius:%1px;}").arg(borderRadius));
    frame->setStyleSheet(qss.join(""));

    QVBoxLayout *verticalLayout = new QVBoxLayout(this);
    verticalLayout->setContentsMargins(0, 0, 0, 0);
    verticalLayout->setSpacing(0);
    verticalLayout->addWidget(frame);

    //将控件按照横向布局排列
    QHBoxLayout *layout = new QHBoxLayout(frame);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(m_vecIP[0]);
    layout->addWidget(m_vecPoint[0]);
    layout->addWidget(m_vecIP[1]);
    layout->addWidget(m_vecPoint[1]);
    layout->addWidget(m_vecIP[2]);
    layout->addWidget(m_vecPoint[2]);
    layout->addWidget(m_vecIP[3]);
}

void MyInputBox::textChanged(const QString &text)
{
    int len=text.length();
    int value=text.toInt();

    if(len==2 && text=="00")
    {
        QLineEdit *senderObj = dynamic_cast<QLineEdit*>(sender());
        if(senderObj)
        {
           senderObj->setText("0");//不存在00的地址
        }
    }
    else if(len==3)
    {
        this->focusNextChild();//移动到下一个位置
    }

    //拼接成完整IP地址
    ip = QString("%1.%2.%3.%4").arg(m_vecIP[0]->text()).arg(m_vecIP[1]->text()).arg(m_vecIP[2]->text()).arg(m_vecIP[3]->text());
}

bool MyInputBox::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        QLineEdit *txt=dynamic_cast<QLineEdit*>(watched);
        if(txt)
        {
            if(txt==m_vecIP[0] || txt==m_vecIP[1] || txt==m_vecIP[2] || txt==m_vecIP[3])
            {
                QKeyEvent *key=(QKeyEvent *)event;

                //如果当前按下了小数点则移动焦点到下一个输入框
                if (key->text() == ".") {
                    this->focusNextChild();
                }

                //如果按下了退格键并且当前文本框已经没有了内容则焦点往前移
                if (key->key() == Qt::Key_Backspace) {
                    if (txt->text().length() <= 1) {
                        this->focusNextPrevChild(false);
                    }
                }
            }
        }

    }

    return QWidget::eventFilter(watched, event);
}


void MyInputBox::clear()
{
    for (int i = 0; i < 4; ++i)
    {
        m_vecIP[i]->clear();
    }
    m_vecIP[0]->setFocus();
}

void MyInputBox::setIPAddress(QString ip)
{
    QStringList parts = ip.split(".");

    if (parts.size() == 4)
    {
        for (int i = 0; i < m_vecIP.size(); ++i)
        {
            m_vecIP[i]->setText(parts[i]);
        }
    }
    else {

    }

}
