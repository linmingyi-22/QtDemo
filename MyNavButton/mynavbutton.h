#ifndef CMYNAVBUTTON_H
#define CMYNAVBUTTON_H

#include <QPushButton>
#include <QPainter>
#include <QDebug>

class CMyNavButton : public QPushButton
{
    Q_OBJECT
public:
    enum TextAlign {
        TextAlign_Left = 0x0001,    //左侧对齐
        TextAlign_Right = 0x0002,   //右侧对齐
        TextAlign_Top = 0x0020,     //顶部对齐
        TextAlign_Bottom = 0x0040,  //底部对齐
        TextAlign_Center = 0x0004   //居中对齐
    };

    enum TrianglePosition {
        TrianglePosition_Left = 0,  //左侧
        TrianglePosition_Right = 1, //右侧
        TrianglePosition_Top = 2,   //顶部
        TrianglePosition_Bottom = 3 //底部
    };

    enum IconPosition {
        IconPosition_Left = 0,      //左侧
        IconPosition_Right = 1,     //右侧
        IconPosition_Top = 2,       //顶部
        IconPosition_Bottom = 3     //底部
    };

    enum LinePosition {
        LinePosition_Left = 0,      //左侧
        LinePosition_Right = 1,     //右侧
        LinePosition_Top = 2,       //顶部
        LinePosition_Bottom = 3     //底部
    };

    explicit CMyNavButton(QWidget *parent = nullptr);

protected:
    void leaveEvent(QEvent *);
    void paintEvent(QPaintEvent *);
    void drawBg(QPainter *painter);
    void drawText(QPainter *painter);
    void drawIcon(QPainter *painter);
    void drawLine(QPainter *painter);
    void drawTriangle(QPainter *painter);

    //私有属性
private:
    int m_iPaddingLeft;            //文字左侧间隔
    int m_iPaddingRight;           //文字右侧间隔
    int m_iPaddingTop;             //文字顶部间隔
    int m_iPaddingBottom;          //文字底部间隔
    TextAlign m_enuTextAlign;        //文字对齐

    bool m_bShowTriangle;          //显示倒三角
    int m_iTriangleLen;            //倒三角边长
    TrianglePosition m_enuTrianglePosition;//倒三角位置
    QColor m_ColorTriangle;       //倒三角颜色

    bool m_bShowIcon;              //显示图标
    int m_iIconSpace;              //图标间隔
    QSize m_SizeIcon;             //图标尺寸
    QPixmap m_PixIconNormal;         //正常图标
    QPixmap m_PixIconHover;          //悬停图标
    QPixmap m_PixIconCheck;          //选中图标

    bool m_bShowLine;              //显示线条
    int m_iLineSpace;              //线条间隔
    int m_iLineWidth;              //线条宽度
    LinePosition m_enuLinePosition;  //线条位置
    QColor m_ColorLine;           //线条颜色

    QColor m_ColorNormalBg;       //正常背景颜色
    QColor m_ColorHoverBg;        //悬停背景颜色
    QColor m_ColorCheckBg;        //选中背景颜色
    QColor m_ColorNormalText;     //正常文字颜色
    QColor m_ColorHoverText;      //悬停文字颜色
    QColor m_ColorCheckText;      //选中文字颜色

    QBrush m_BrushNormalBg;       //正常背景画刷
    QBrush m_BrushHoverBg;        //悬停背景画刷
    QBrush m_BrushCheckBg;        //选中背景画刷

    bool m_bHover;                 //悬停标志位

    //私有属性接口
public:
    //获取和设置文字左侧间隔
    int getPaddingLeft() const;
    void setPaddingLeft(int paddingLeft);

    //获取和设置文字左侧间隔
    int getPaddingRight() const;
    void setPaddingRight(int paddingRight);

    //获取和设置文字顶部间隔
    int getPaddingTop() const;
    void setPaddingTop(int paddingTop);

    //获取和设置文字底部间隔
    int getPaddingBottom() const;
    void setPaddingBottom(int paddingBottom);

    //设置边距
    void setPadding(int padding);
    void setPadding(int paddingLeft, int paddingRight, int paddingTop, int paddingBottom);

    //获取和设置文字对齐
    TextAlign getTextAlign() const;
    void setTextAlign(const TextAlign &textAlign);

    //获取和设置显示倒三角
    bool getShowTriangle() const;
    void setShowTriangle(bool showTriangle);

    //获取和设置倒三角边长
    int getTriangleLen() const;
    void setTriangleLen(int triangleLen);

    //获取和设置倒三角位置
    TrianglePosition getTrianglePosition() const;
    void setTrianglePosition(const TrianglePosition &trianglePosition);

    //获取和设置倒三角颜色
    QColor getTriangleColor() const;
    void setTriangleColor(const QColor &triangleColor);

    //获取和设置显示图标
    bool getShowIcon() const;
    void setShowIcon(bool showIcon);

    //获取和设置图标间隔
    int getIconSpace() const;
    void setIconSpace(int iconSpace);

    //获取和设置图标尺寸
    QSize getIconSize() const;
    void setIconSize(const QSize &iconSize);

    //获取和设置正常图标
    QPixmap getIconNormal() const;
    void setIconNormal(const QPixmap &iconNormal);

    //获取和设置悬停图标
    QPixmap getIconHover() const;
    void setIconHover(const QPixmap &iconHover);

    //获取和设置按下图标
    QPixmap getIconCheck() const;
    void setIconCheck(const QPixmap &iconCheck);

    //获取和设置显示线条
    bool getShowLine() const;
    void setShowLine(bool showLine);

    //获取和设置线条间隔
    int getLineSpace() const;
    void setLineSpace(int lineSpace);

    //获取和设置线条宽度
    int getLineWidth() const;
    void setLineWidth(int lineWidth);

    //获取和设置线条位置
    LinePosition getLinePosition() const;
    void setLinePosition(const LinePosition &linePosition);

    //获取和设置线条颜色
    QColor getLineColor() const;
    void setLineColor(const QColor &lineColor);

    //获取和设置正常背景颜色
    QColor getNormalBgColor() const;
    void setNormalBgColor(const QColor &normalBgColor);

    //获取和设置悬停背景颜色
    QColor getHoverBgColor() const;
    void setHoverBgColor(const QColor &hoverBgColor);

    //获取和设置选中背景颜色
    QColor getCheckBgColor() const;
    void setCheckBgColor(const QColor &checkBgColor);

    //获取和设置正常文字颜色
    QColor getNormalTextColor() const;
    void setNormalTextColor(const QColor &normalTextColor);

    //获取和设置悬停文字颜色
    QColor getHoverTextColor() const;
    void setHoverTextColor(const QColor &hoverTextColor);

    //获取和设置选中文字颜色
    QColor getCheckTextColor() const;
    void setCheckTextColor(const QColor &checkTextColor);

    //设置正常背景画刷
    void setNormalBgBrush(const QBrush &normalBgBrush);
    //设置悬停背景画刷
    void setHoverBgBrush(const QBrush &hoverBgBrush);
    //设置选中背景画刷
    void setCheckBgBrush(const QBrush &checkBgBrush);
signals:

};

#endif // CMYNAVBUTTON_H
