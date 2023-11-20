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

//最大新建页面数(new n)
#define MAXPAGE 5

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
    void on_action_New_triggered();

    void on_tabWidget_tabCloseRequested(int index);

    void on_action_Open_triggered();

    void on_action_Save_triggered();

    void on_actionl_SaveAs_triggered();

private:
    Ui::CMyTextEditor *ui;

    bool bPageNew[MAXPAGE];//可新建的页面
    int iPageCount;//总页面数量

    //QMap<QWidget*,QTextEdit*> m_mapWidgt2TextEdit_New;//一一对应,新增的页面
    //QMap<QWidget*,QTextEdit*> m_mapWidgt2TextEdit_File;//一一对应,打开文件的页面
    QMap<QWidget*,strTabPage*> m_mapWidget2TextEdit_Total;//合二为一，通过bNew来判断
};
#endif // CMYTEXTEDITOR_H
