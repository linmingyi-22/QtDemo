#include "mynavbutton.h"

CMyNavButton::CMyNavButton(QWidget *parent)
    : QPushButton{parent}
{
    m_iPaddingLeft=20;
    m_iPaddingRight=5;
    m_iPaddingTop=5;
    m_iPaddingBottom=5;

    m_bShowTriangle=false;
    m_iTriangleLen=5;
    m_enuTrianglePosition=TrianglePosition_Right;
    m_ColorTriangle=QColor(255,255,255);

    m_bShowIcon=false;
    m_iIconSpace=10;
    m_SizeIcon=QSize(16,16);
    m_PixIconNormal=QPixmap();
    m_PixIconHover=QPixmap();
    m_PixIconCheck=QPixmap();

    //线条相关
    m_bShowLine=true;
    m_iLineSpace=0;
    m_iLineWidth=5;
    m_enuLinePosition=LinePosition_Left;
    m_ColorLine=QColor(0, 187, 158);

    m_ColorNormalBg=QColor(230, 230, 230);
    m_ColorHoverBg=QColor(130, 130, 130);
    m_ColorCheckBg=QColor(80, 80, 80);

    m_ColorNormalText=QColor(100, 100, 100);
    m_ColorHoverText=QColor(255, 255, 255);
    m_ColorCheckText=QColor(255, 255, 255);

    m_BrushNormalBg=Qt::NoBrush;
    m_BrushHoverBg=Qt::NoBrush;
    m_BrushCheckBg=Qt::NoBrush;

    m_bHover=false;
    setCheckable(true);
}

void CMyNavButton::leaveEvent(QEvent *)
{
    m_bHover=false;
    this->update();
}

void CMyNavButton::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    //绘制背景
    drawBg(&painter);
    //绘制文字
    drawText(&painter);
    //绘制图标
    drawIcon(&painter);
    //绘制边框线条
    drawLine(&painter);
    //绘制倒三角
    drawTriangle(&painter);
}

void CMyNavButton::drawBg(QPainter *painter)
{
    painter->save();
    painter->setPen(Qt::NoPen);

    int width=this->width();
    int height=this->height();

    QRect rect;
    if(m_enuLinePosition==LinePosition_Left)
    {
        rect=QRect(m_iLineSpace,0,width-m_iLineSpace,height);
    }
    else if (m_enuLinePosition==LinePosition_Right)
    {
        rect=QRect(0,0,width-m_iLineSpace,height);
    }
    else if (m_enuLinePosition==LinePosition_Top)
    {
        rect=QRect(0,m_iLineSpace,width,height-m_iLineSpace);
    }
    else if (m_enuLinePosition==LinePosition_Bottom)
    {
        rect=QRect(0,0,width,height-m_iLineSpace);
    }

    //按照不同状态选择不同画刷
    QBrush brush(Qt::NoBrush);
    if(isChecked())
    {
        brush=m_BrushCheckBg;
    }
    else if(m_bHover)
    {
        brush=m_BrushHoverBg;
    }
    else
    {
        brush=m_BrushNormalBg;
    }

    if(brush!=Qt::NoBrush)
    {
        painter->setBrush(brush);
    }
    else
    {
        QColor color(0, 0, 0);
        if(isChecked())
        {
            color=m_ColorCheckBg;
        }
        else if(m_bHover) {
            color=m_ColorHoverBg;
        }
        else
        {
            color=m_ColorNormalBg;
        }

        painter->setBrush(color);
    }

    painter->drawRect(rect);

    painter->restore();
}


void CMyNavButton::drawText(QPainter *painter)
{
    painter->save();
    painter->setBrush(Qt::NoBrush);

    QColor textColor(0,0,0);
    if(isChecked())
    {
        textColor=m_ColorCheckText;
    }
    else if(m_bHover)
    {
        textColor=m_ColorHoverText;
    }
    else
    {
        textColor=m_ColorNormalText;
    }

    QRect textRect = QRect(m_iPaddingLeft, m_iPaddingTop, width() - m_iPaddingLeft - m_iPaddingRight, height() - m_iPaddingTop - m_iPaddingBottom);
    painter->setPen(textColor);
    painter->drawText(textRect, m_enuTextAlign | Qt::AlignVCenter, text());

    painter->restore();
}

void CMyNavButton::drawIcon(QPainter *painter)
{
    if(!m_bShowIcon)
        return;

    painter->save();

    QPixmap pixmap;
    if(isChecked())
    {
        pixmap=m_PixIconCheck;
    }
    else if(m_bHover)
    {
        pixmap=m_PixIconHover;
    }
    else
    {
        pixmap=m_PixIconNormal;
    }

    if(!pixmap.isNull())
    {
        pixmap=pixmap.scaled(m_SizeIcon,Qt::KeepAspectRatio,Qt::SmoothTransformation);
        painter->drawPixmap(m_iIconSpace,(height()-m_SizeIcon.height())/2,pixmap);
    }

    painter->restore();
}

void CMyNavButton::drawLine(QPainter *painter)
{
    if(!m_bShowLine)
        return;

    if(!isChecked())
        return;

    painter->save();

    QPen pen;
    pen.setWidth(m_iLineWidth);
    pen.setColor(m_ColorLine);
    painter->setPen(pen);

    QPoint start,end;
    if(m_enuLinePosition==LinePosition_Left)
    {
        start=QPoint(0,0);
        end=QPoint(0,height());
    }
    else if(m_enuLinePosition==LinePosition_Right)
    {
        start=QPoint(width(), 0);
        end=QPoint(width(), height());
    }
    else if(m_enuLinePosition==LinePosition_Top)
    {
        start=QPoint(0, 0);
        end=QPoint(width(), 0);
    }
    else if(m_enuLinePosition==LinePosition_Bottom)
    {
        start=QPoint(0, height());
        end=QPoint(width(), height());
    }
    painter->drawLine(start,end);

    painter->restore();
}

void CMyNavButton::drawTriangle(QPainter *painter)
{
    if(!m_bShowTriangle)
        return;

    if(!isChecked())
        return;

    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setBrush(m_ColorTriangle);

    int width=this->width();
    int height=this->height();
    int midWidth=width/2;
    int midHeight=height/2;

    QPolygon polygon;
    if(m_enuTrianglePosition==TrianglePosition::TrianglePosition_Left)
    {
        polygon.setPoints(3,m_iTriangleLen,midHeight,0,midHeight-m_iTriangleLen,0,midHeight+m_iTriangleLen);
    }
    else if(m_enuTrianglePosition == TrianglePosition_Right)
    {
        polygon.setPoints(3,width-m_iTriangleLen,midHeight,width,midHeight-m_iTriangleLen,width,midHeight+m_iTriangleLen);
    }
    else if(m_enuTrianglePosition == TrianglePosition_Top)
    {
        polygon.setPoints(3,midWidth,m_iTriangleLen,midWidth-m_iTriangleLen,0,midWidth+m_iTriangleLen,0);
    }
    else if(m_enuTrianglePosition == TrianglePosition_Bottom)
    {
        polygon.setPoints(3,midWidth,height-m_iTriangleLen,midWidth-m_iTriangleLen,height,midWidth+m_iTriangleLen,height);
    }

    painter->drawPolygon(polygon);

    painter->restore();
}

int CMyNavButton::getPaddingLeft() const
{
    return this->m_iPaddingLeft;
}

void CMyNavButton::setPaddingLeft(int paddingLeft)
{
    if (this->m_iPaddingLeft != paddingLeft) {
        this->m_iPaddingLeft = paddingLeft;
        this->update();
    }
}

//获取和设置文字左侧间隔
int CMyNavButton::getPaddingRight() const
{
    return this->m_iPaddingRight;
}

void CMyNavButton::setPaddingRight(int paddingRight)
{
    if(this->m_iPaddingRight!=paddingRight)
    {
        this->m_iPaddingRight=paddingRight;
        this->update();
    }
}

//获取和设置文字顶部间隔
int CMyNavButton::getPaddingTop() const
{
    return this->m_iPaddingTop;
}

void CMyNavButton::setPaddingTop(int paddingTop)
{
    if(this->m_iPaddingTop!=paddingTop)
    {
        this->m_iPaddingTop=paddingTop;
        this->update();
    }
}

//获取和设置文字底部间隔
int CMyNavButton::getPaddingBottom() const
{
    return this->m_iPaddingBottom;
}

void CMyNavButton::setPaddingBottom(int paddingBottom)
{
    if(this->m_iPaddingBottom!=paddingBottom)
    {
        this->m_iPaddingBottom=paddingBottom;
        this->update();
    }
}

//设置边距
void CMyNavButton::setPadding(int padding)
{
    setPadding(padding,padding,padding,padding);
}

void CMyNavButton::setPadding(int paddingLeft, int paddingRight, int paddingTop, int paddingBottom)
{
    this->m_iPaddingLeft=paddingLeft;
    this->m_iPaddingRight=paddingRight;
    this->m_iPaddingTop=paddingTop;
    this->m_iPaddingBottom=paddingBottom;
    this->update();
}

//获取和设置文字对齐
CMyNavButton::TextAlign CMyNavButton::getTextAlign() const
{
    return this->m_enuTextAlign;
}

void CMyNavButton::setTextAlign(const CMyNavButton::TextAlign &textAlign)
{
    if(this->m_enuTextAlign!=textAlign)
    {
        this->m_enuTextAlign=textAlign;
        this->update();
    }
}

//获取和设置显示倒三角
bool CMyNavButton::getShowTriangle() const
{
    return this->m_bShowTriangle;
}

void CMyNavButton::setShowTriangle(bool showTriangle)
{
    if(this->m_bShowTriangle!=showTriangle)
    {
        this->m_bShowTriangle=showTriangle;
        this->update();
    }
}

//获取和设置倒三角边长
int CMyNavButton::getTriangleLen() const
{
    return this->m_iTriangleLen;
}

void CMyNavButton::setTriangleLen(int triangleLen)
{
    if(this->m_iTriangleLen!=triangleLen)
    {
        this->m_iTriangleLen=triangleLen;
        this->update();
    }
}

//获取和设置倒三角位置
CMyNavButton::TrianglePosition CMyNavButton::getTrianglePosition() const
{
    return this->m_enuTrianglePosition;
}

void CMyNavButton::setTrianglePosition(const CMyNavButton::TrianglePosition &trianglePosition)
{
    if(this->m_enuTrianglePosition!=trianglePosition)
    {
        this->m_enuTrianglePosition=trianglePosition;
        this->update();
    }
}

//获取和设置倒三角颜色
QColor CMyNavButton::getTriangleColor() const
{
    return this->m_ColorTriangle;
}

void CMyNavButton::setTriangleColor(const QColor &triangleColor)
{
    if(this->m_ColorTriangle!=triangleColor)
    {
        this->m_ColorTriangle=triangleColor;
        this->update();
    }
}

//获取和设置显示图标
bool CMyNavButton::getShowIcon() const
{
    return this->m_bShowIcon;
}

void CMyNavButton::setShowIcon(bool showIcon)
{
    if (this->m_bShowIcon!=showIcon)
    {
        this->m_bShowIcon=showIcon;
        this->update();
    }
}

//获取和设置图标间隔
int CMyNavButton::getIconSpace() const
{
    return this->m_iIconSpace;
}

void CMyNavButton::setIconSpace(int iconSpace)
{
    if (this->m_iIconSpace!=iconSpace)
    {
        this->m_iIconSpace=iconSpace;
        this->update();
    }
}

//获取和设置图标尺寸
QSize CMyNavButton::getIconSize() const
{
    return this->m_SizeIcon;
}

void CMyNavButton::setIconSize(const QSize &iconSize)
{
    if (this->m_SizeIcon!=iconSize)
    {
        this->m_SizeIcon=iconSize;
        this->update();
    }
}

//获取和设置正常图标
QPixmap CMyNavButton::getIconNormal() const
{
    return this->m_PixIconNormal;
}

void CMyNavButton::setIconNormal(const QPixmap &iconNormal)
{
    this->m_PixIconNormal = iconNormal;
    this->update();
}

//获取和设置悬停图标
QPixmap CMyNavButton::getIconHover() const
{
    return this->m_PixIconHover;
}

void CMyNavButton::setIconHover(const QPixmap &iconHover)
{
    this->m_PixIconHover = iconHover;
    this->update();
}

//获取和设置按下图标
QPixmap CMyNavButton::getIconCheck() const
{
    return this->m_PixIconCheck;
}

void CMyNavButton::setIconCheck(const QPixmap &iconCheck)
{
    this->m_PixIconCheck = iconCheck;
    this->update();
}

//获取和设置显示线条
bool CMyNavButton::getShowLine() const
{
    return this->m_bShowLine;
}

void CMyNavButton::setShowLine(bool showLine)
{
    if (this->m_bShowLine!=showLine)
    {
        this->m_bShowLine=showLine;
        this->update();
    }
}

//获取和设置线条间隔
int CMyNavButton::getLineSpace() const
{
    return this->m_iLineSpace;
}

void CMyNavButton::setLineSpace(int lineSpace)
{
    if (this->m_iLineSpace!=lineSpace)
    {
        this->m_iLineSpace=lineSpace;
        this->update();
    }
}

//获取和设置线条宽度
int CMyNavButton::getLineWidth() const
{
    return this->m_iLineWidth;
}

void CMyNavButton::setLineWidth(int lineWidth)
{
    if (this->m_iLineWidth!=lineWidth)
    {
        this->m_iLineWidth=lineWidth;
        this->update();
    }
}

//获取和设置线条位置
CMyNavButton::LinePosition CMyNavButton::getLinePosition() const
{
    return this->m_enuLinePosition;
}

void CMyNavButton::setLinePosition(const CMyNavButton::LinePosition &linePosition)
{
    if (this->m_enuLinePosition!=linePosition)
    {
        this->m_enuLinePosition=linePosition;
        this->update();
    }
}

//获取和设置线条颜色
QColor CMyNavButton::getLineColor() const
{
    return this->m_ColorLine;
}

void CMyNavButton::setLineColor(const QColor &lineColor)
{
    if (this->m_ColorLine!=lineColor)
    {
        this->m_ColorLine=lineColor;
        this->update();
    }
}

//获取和设置正常背景颜色
QColor CMyNavButton::getNormalBgColor() const
{
    return this->m_ColorNormalBg;
}

void CMyNavButton::setNormalBgColor(const QColor &normalBgColor)
{
    if (this->m_ColorNormalBg!=normalBgColor)
    {
        this->m_ColorNormalBg=normalBgColor;
        this->update();
    }
}

//获取和设置悬停背景颜色
QColor CMyNavButton::getHoverBgColor() const
{
    return this->m_ColorHoverBg;
}

void CMyNavButton::setHoverBgColor(const QColor &hoverBgColor)
{
    if (this->m_ColorHoverBg!=hoverBgColor)
    {
        this->m_ColorHoverBg=hoverBgColor;
        this->update();
    }
}

//获取和设置选中背景颜色
QColor CMyNavButton::getCheckBgColor() const
{
    return this->m_ColorCheckBg;
}

void CMyNavButton::setCheckBgColor(const QColor &checkBgColor)
{
    if (this->m_ColorCheckBg!=checkBgColor)
    {
        this->m_ColorCheckBg=checkBgColor;
        this->update();
    }
}

//获取和设置正常文字颜色
QColor CMyNavButton::getNormalTextColor() const
{
    return this->m_ColorNormalText;
}

void CMyNavButton::setNormalTextColor(const QColor &normalTextColor)
{
    if (this->m_ColorNormalText!=normalTextColor)
    {
        this->m_ColorNormalText=normalTextColor;
        this->update();
    }
}

//获取和设置悬停文字颜色
QColor CMyNavButton::getHoverTextColor() const
{
    return this->m_ColorHoverText;
}

void CMyNavButton::setHoverTextColor(const QColor &hoverTextColor)
{
    if (this->m_ColorHoverText!=hoverTextColor)
    {
        this->m_ColorHoverText=hoverTextColor;
        this->update();
    }
}

//获取和设置选中文字颜色
QColor CMyNavButton::getCheckTextColor() const
{
    return this->m_ColorCheckText;
}

void CMyNavButton::setCheckTextColor(const QColor &checkTextColor)
{
    if (this->m_ColorCheckText!=checkTextColor)
    {
        this->m_ColorCheckText=checkTextColor;
        this->update();
    }
}

//设置正常背景画刷
void CMyNavButton::setNormalBgBrush(const QBrush &normalBgBrush)
{
    if (this->m_BrushNormalBg!=normalBgBrush)
    {
        this->m_BrushNormalBg=normalBgBrush;
        this->update();
    }
}

//设置悬停背景画刷
void CMyNavButton::setHoverBgBrush(const QBrush &hoverBgBrush)
{
    if (this->m_BrushHoverBg!=hoverBgBrush)
    {
        this->m_BrushHoverBg=hoverBgBrush;
        this->update();
    }
}

//设置选中背景画刷
void CMyNavButton::setCheckBgBrush(const QBrush &checkBgBrush)
{
    if (this->m_BrushCheckBg!=checkBgBrush)
    {
        this->m_BrushCheckBg=checkBgBrush;
        this->update();
    }
}
