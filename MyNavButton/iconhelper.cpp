#include "iconhelper.h"

IconHelper *IconHelper::iconFontAliBaBa = 0;
IconHelper *IconHelper::iconFontAwesome = 0;
IconHelper *IconHelper::iconFontAwesome6 = 0;
IconHelper *IconHelper::iconFontWeather = 0;
int IconHelper::iconFontIndex = -1;


IconHelper::IconHelper(const QString &fontFile, const QString &fontName, QObject *parent)
    : QObject{parent}
{
    QFontDatabase fontDB;
    if(fontDB.families().contains(fontName) && QFile(fontFile).exists())//当前字体数据库中不存在该字体，并且该字体所在的文件是存在的
    {
        int fontId=fontDB.addApplicationFont(fontFile);//将指定的字体文件（fontFile）添加到应用程序的字体库中，并返回一个字体标识符（fontId）
        QStringList listName=fontDB.applicationFontFamilies(fontId);
        qDebug()<<"11"<<listName<<Qt::endl;
        QStringList fontFamilies = fontDB.families();
        qDebug()<<"22"<<fontFamilies<<Qt::endl;
        if(listName.count()==0)
        {
            qDebug()<< QString("load %1 error").arg(fontName);
        }
    }

    if (fontDB.families().contains(fontName))
        m_fontIcon=QFont(fontName);
#if (QT_VERSION >= QT_VERSION_CHECK(4,8,0))
        m_fontIcon.setHintingPreference(QFont::PreferNoHinting);
#endif
}

void IconHelper::initFont()
{
    static bool isInit=false;

    if(!isInit)//初始化
    {
        isInit=true;
        if (iconFontAliBaBa == 0) {
            iconFontAliBaBa = new IconHelper(":/font/iconfont.ttf", "iconfont");
        }
        if (iconFontAwesome == 0) {
            iconFontAwesome = new IconHelper(":/font/fontawesome-webfont.ttf", "FontAwesome");
        }
        if (iconFontAwesome6 == 0) {
            iconFontAwesome6 = new IconHelper(":/font/fa-regular-400.ttf", "Font Awesome 6 Pro Regular");
        }
        if (iconFontWeather == 0) {
            iconFontWeather = new IconHelper(":/font/pe-icon-set-weather.ttf", "pe-icon-set-weather");
        }

    }
}

IconHelper *IconHelper::getIconHelper(int icon)
{
    initFont();

    IconHelper *iconHelper=iconFontAwesome;
    if(iconFontIndex<0)
    {
        if((icon>=0xe501 && icon<=0xe793)||(icon>=0xe8d5 && icon<=0xea5d)||(icon>=0xeb00 && icon<=0xec00))
        {
            iconHelper = iconFontAliBaBa;
        }
    }
    else if (iconFontIndex == 0)
    {
        iconHelper = iconFontAliBaBa;
    }
    else if (iconFontIndex == 1)
    {
        iconHelper = iconFontAwesome;
    }
    else if (iconFontIndex == 2)
    {
        iconHelper = iconFontAwesome6;
    }
    else if (iconFontIndex == 3)
    {
        iconHelper = iconFontWeather;
    }

    return iconHelper;
}

void IconHelper::setIcon(QLabel *lab, int icon, quint32 size)
{
    getIconHelper(icon)->setIcon1(lab,icon,size);
}

void IconHelper::setIcon(QAbstractButton *btn, int icon, quint32 size)
{
    getIconHelper(icon)->setIcon1(btn,icon,size);
}

//设置引用图形字体文件索引
void IconHelper::setIconFontIndex(int index)
{
    iconFontIndex=index;
}

//获取图形字体
QFont IconHelper::getIconFontAliBaBa()
{
    initFont();
    return iconFontAliBaBa->getIconFont();
}

QFont IconHelper::getIconFontAwesome()
{
    initFont();
    return iconFontAwesome->getIconFont();
}

QFont IconHelper::getIconFontAwesome6()
{
    initFont();
    return iconFontAwesome6->getIconFont();
}

QFont IconHelper::getIconFontWeather()
{
    initFont();
    return iconFontWeather->getIconFont();
}

QPixmap IconHelper::getPixmap(const QColor &color, int icon, quint32 size, quint32 width, quint32 height, int flags)
{
    return getIconHelper(icon)->getPixmap1(color, icon, size, width, height, flags);
}

void IconHelper::setStyle(QWidget *widget, QList<QPushButton *> btns, QList<int> icons, const StyleColor &styleColor)
{
    int icon=icons.first();
    getIconHelper(icon)->setStyle1(widget, btns, icons, styleColor);
}

void IconHelper::setStyle(QWidget *widget, QList<QToolButton *> btns, QList<int> icons, const StyleColor &styleColor)
{
    int icon=icons.first();
    getIconHelper(icon)->setStyle1(widget, btns, icons, styleColor);
}

void IconHelper::setStyle(QWidget *widget, QList<QAbstractButton *> btns, QList<int> icons, const StyleColor &styleColor)
{
    int btnCount=btns.count();
    int iconCount=icons.count();

    QString position = styleColor.position;
    quint32 btnWidth = styleColor.btnWidth;
    quint32 btnHeight = styleColor.btnHeight;
    quint32 iconSize = styleColor.iconSize;
    quint32 iconWidth = styleColor.iconWidth;
    quint32 iconHeight = styleColor.iconHeight;
    quint32 borderWidth = styleColor.borderWidth;

    //根据不同的位置计算边框
    QString strBorder;
    if(position=="top")
    {
        strBorder = QString("border-width:%1px 0px 0px 0px;padding-top:%1px;padding-bottom:%2px;").arg(borderWidth).arg(borderWidth * 2);
    }
    else if(position=="right")
    {
        strBorder = QString("border-width:0px %1px 0px 0px;padding-right:%1px;padding-left:%2px;").arg(borderWidth).arg(borderWidth * 2);
    }
    else if(position=="bottom")
    {
        strBorder = QString("border-width:0px 0px %1px 0px;padding-bottom:%1px;padding-top:%2px;").arg(borderWidth).arg(borderWidth * 2);
    }
    else if(position=="left")
    {
        strBorder = QString("border-width:0px 0px 0px %1px;padding-left:%1px;padding-right:%2px;").arg(borderWidth).arg(borderWidth * 2);
    }
}

bool IconHelper::eventFilter(QObject *watched, QEvent *event)
{

}

void IconHelper::toggled(bool checked)
{

}

QFont IconHelper::getIconFont()
{

}

void IconHelper::setIcon1(QLabel *lab, int icon, quint32 size)
{
    m_fontIcon.setPixelSize(size);
    lab->setFont(m_fontIcon);
    lab->setText((QChar)icon);
}

void IconHelper::setIcon1(QAbstractButton *btn, int icon, quint32 size)
{
    m_fontIcon.setPixelSize(size);
    btn->setFont(m_fontIcon);
    btn->setText((QChar)icon);
}

void IconHelper::setPixmap1(QAbstractButton *btn, const QColor &color, int icon, quint32 size, quint32 width, quint32 height, int flags)
{

}

void IconHelper::setPixmap(QAbstractButton *btn,const QColor &color,int icon,quint32 size,quint32 width, quint32 height,int flags)
{
    getIconHelper(icon)->setPixmap1(btn, color, icon, size, width, height, flags);
}

//获取图标图片
QPixmap IconHelper::getPixmap1(const QColor &color, int icon, quint32 size,quint32 width, quint32 height,int flags)
{
    QPixmap pix(width,height);

    pix.fill(Qt::transparent);//使用透明颜色填充

    QPainter painter(&pix);//指定绘制目标
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.setPen(color);

    m_fontIcon.setPixelSize(size);
    painter.setFont(m_fontIcon);
    painter.drawText(pix.rect(), flags, (QChar)icon);//icon表示的是图标（箭头....）
    //painter.drawText(pix.rect(), flags, "11");
    painter.end();
    return pix;
}

void IconHelper::setStyle1(QWidget *widget, QList<QPushButton *> btns, QList<int> icons, const StyleColor &styleColor)
{
    QList<QAbstractButton *> list;
    foreach(QPushButton *btn,btns) {
        list << btn;
    }

    setStyle(widget, list, icons, styleColor);
}

void IconHelper::setStyle1(QWidget *widget, QList<QToolButton *> btns, QList<int> icons, const StyleColor &styleColor)
{
    QList<QAbstractButton *> list;
    foreach(QToolButton *btn,btns) {
        list << btn;
    }

    setStyle(widget, list, icons, styleColor);
}

void IconHelper::setStyle1(QWidget *widget, QList<QAbstractButton *> btns, QList<int> icons, const StyleColor &styleColor)
{

}
