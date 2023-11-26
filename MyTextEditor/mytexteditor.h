#ifndef CMYTEXTEDITOR_H
#define CMYTEXTEDITOR_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextEdit>
#include <QFile>
#include <QString>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QAction>
#include <QFontDialog>
#include <QColorDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#define FilePath    "filePath"
#define FontFamily  "fontFamily"
#define FontSize    "fontSize"
#define FontWeight  "fontWeight"
#define Underline   "underline"
#define Strikeout   "strikeout"
#define Italic      "italic"

//最大新建页面数(new n)
#define MAXPAGE 5

enum OptFontType
{
    Opt_Selected=0,
    Opt_All,
};

struct strTabPage //一个tabPage存在的东西
{
    bool bNew;//是否为空的tabPage(new n)
    QTextEdit *textEidt;//页面中的东西
    QString filePath;

    strTabPage()
    {
        bNew=true;
        textEidt=new QTextEdit;
        filePath="";
    }
};


QT_BEGIN_NAMESPACE
namespace Ui { class CMyTextEditor; }
QT_END_NAMESPACE

class CMyTextEditor : public QMainWindow
{
    Q_OBJECT

public:
    CMyTextEditor(QWidget *parent = nullptr);
    ~CMyTextEditor();

    QWidget* createTabPage(QString filename="");
    bool saveFile(const QString fileName);
    bool saveAsFile(const QString fileName);

protected:
    void keyPressEvent(QKeyEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent * event);

private slots:
    void findTextBackward();
    void findTextForward();

    void saveFileFont(QWidget *widget);
    void loadFileFont(QWidget *widget);

    void setFontFamily(QString strFontFamily,QWidget *widget=nullptr,int iType=Opt_Selected);
    void setFontSize(int iFontSize,QWidget *widget=nullptr,int iType=Opt_Selected);
    void setFontWeight(int iWeight,QWidget *widget=nullptr,int iType=Opt_Selected);
    void setUnderline(bool bUnderline=true,QWidget *widget=nullptr,int iType=Opt_Selected);
    void setStrikeout(bool bStrikeout=true,QWidget *widget=nullptr,int iType=Opt_Selected);
    void setItalic(bool bItalic=true,QWidget *widget=nullptr,int iType=Opt_Selected);


private slots:
    void on_action_New_triggered();

    void on_tabWidget_tabCloseRequested(int index);

    void on_action_Open_triggered();

    void on_action_Save_triggered();

    void on_actionl_SaveAs_triggered();

    void on_action_Copy_triggered();

    void on_action_Paste_triggered();

    void on_action_Undo_triggered();

    void on_action_Cut_triggered();

    void on_action_Find_triggered();

    void on_action_TextBold_triggered();

    void on_action_TextType_triggered();

    void on_action_TextItalic_triggered();

    void on_action_TextStrike_triggered();

    void on_action_TextUnder_triggered();

    void on_action_About_triggered();

    void on_action_color_triggered();

    void on_action_bgColor_triggered();

private:
    Ui::CMyTextEditor *ui;

    bool bPageNew[MAXPAGE];//可新建的页面
    int iPageCount;//总页面数量

    //QMap<QWidget*,QTextEdit*> m_mapWidgt2TextEdit_New;//一一对应,新增的页面
    //QMap<QWidget*,QTextEdit*> m_mapWidgt2TextEdit_File;//一一对应,打开文件的页面
    QMap<QWidget*,strTabPage*> m_mapWidget2TextEdit_Total;//合二为一，通过bNew来判断

    QDialog *DlgFind;
    QLineEdit *leFind;
    QLabel *lblFind;

    QFontDialog *fontDialog;
    const QString strFileJson="./fontType.json";
    QColorDialog *colorDialog_font;
    QColorDialog *colorDialog_fontBg;
};
#endif // CMYTEXTEDITOR_H
