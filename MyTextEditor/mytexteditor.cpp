#include "mytexteditor.h"
#include "ui_mytexteditor.h"

CMyTextEditor::CMyTextEditor(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CMyTextEditor)
{
    ui->setupUi(this);
    setAcceptDrops(true);


    //新建的页面
    for (int i = 0; i < MAXPAGE; ++i)
    {
        bPageNew[i]=false;
    }

    //默认有一个空界面
    bPageNew[0]=true;
    iPageCount=1;
    strTabPage *tabPage=new strTabPage;
    tabPage->bNew=true;
    ui->textEdit_1->setAcceptDrops(false);//禁用拖拽事件，主控件开启即可
    tabPage->textEidt=ui->textEdit_1;
    tabPage->filePath="";
    m_mapWidget2TextEdit_Total.insert(ui->tab_1,tabPage);

}

CMyTextEditor::~CMyTextEditor()
{
    delete ui;
}

QWidget* CMyTextEditor::createTabPage(QString filename)
{
    if(filename=="")//新建界面(new n)
    {
        int iPageUse=-1;
        for (int i = 0; i < MAXPAGE; ++i)
        {
            if(bPageNew[i]==false)
            {
                iPageUse=i+1;
                bPageNew[i]=true;
                break;
            }
        }

        if(iPageUse!=-1)
        {
            QWidget *tab_new=new QWidget(ui->tabWidget);
            tab_new->setObjectName(QString("tab_%1").arg(iPageUse));

            QTextEdit *td_new=new QTextEdit(tab_new);
            td_new->setObjectName(QString("textEdit_%1").arg(iPageUse));
            tab_new->setLayout(new QVBoxLayout());
            tab_new->layout()->addWidget(td_new);

            ui->tabWidget->addTab(tab_new,QString("new %1").arg(iPageUse));

            this->iPageCount++;
            strTabPage *tabPage=new strTabPage;
            tabPage->bNew=true;
            tabPage->textEidt=td_new;
            tabPage->filePath="";
            m_mapWidget2TextEdit_Total.insert(tab_new,tabPage);
            return tab_new;
        }
        else
        {
            QMessageBox::warning(this,"注意","已经达到最大页面数");
            return nullptr;
        }
    }
    else//新建界面(打开文件)
    {
        QWidget *tab_new=new QWidget(ui->tabWidget);
        tab_new->setObjectName(QString("tab_%1").arg(this->iPageCount));

        QTextEdit *td_new=new QTextEdit(tab_new);
        td_new->setObjectName(QString("textEdit_%1").arg(this->iPageCount));
        tab_new->setLayout(new QVBoxLayout());
        tab_new->layout()->addWidget(td_new);

        ui->tabWidget->addTab(tab_new,filename.split("/").back());
        this->iPageCount++;

        strTabPage *tabPage=new strTabPage;
        tabPage->bNew=false;
        tabPage->textEidt=td_new;
        tabPage->filePath=filename;
        m_mapWidget2TextEdit_Total.insert(tab_new,tabPage);
        return tab_new;
    }

}

//保存文件
//使用场景：1.正常保存；2.改变内容后非new类型文件关闭；3.另存为
bool CMyTextEditor::saveFile(const QString fileName)
{
    QFile file(fileName);

    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this,"警告",QString("无法写入文件 %1：/n %2").arg(fileName).arg(file.errorString()));
        return false;
    }
    QTextStream out(&file);
    // 鼠标指针变为等待状态
    QApplication::setOverrideCursor(Qt::WaitCursor);
    out<<m_mapWidget2TextEdit_Total[ui->tabWidget->currentWidget()]->textEidt->toPlainText();
    // 鼠标指针恢复原来的状态
    QApplication::restoreOverrideCursor();
    return true;
}

//另存为文件
//使用场景：1.正常使用；2.改变内容后new类型文件关闭
bool CMyTextEditor::saveAsFile(const QString fileName)
{

    if (fileName.isEmpty())
    {
        QMessageBox::warning(this,"警告",QString("另存为失败"));
        return false;
    }
    bool bSaveSuc=saveFile(fileName);
    if(bSaveSuc)
    {
        QWidget *tab_save = ui->tabWidget->currentWidget();

        if(m_mapWidget2TextEdit_Total[tab_save]->bNew)//new类型
        {
            m_mapWidget2TextEdit_Total[tab_save]->bNew=false;

            QString name=tab_save->objectName();
            name=name.split("_").at(1);
            bPageNew[std::stoi(name.toStdString())-1]=false;
        }

        m_mapWidget2TextEdit_Total[tab_save]->filePath=fileName;
        int iIndex=ui->tabWidget->indexOf(tab_save);
        ui->tabWidget->setTabText(iIndex,fileName.split("/").back());
    }
    return bSaveSuc;
}

void CMyTextEditor::keyPressEvent(QKeyEvent *event)
{
    if(event->modifiers() & Qt::AltModifier)
    {
        if (event->key() == Qt::Key_F) {
            menuBar()->setActiveAction(ui->menu_file->menuAction());
        }
        else if (event->key() == Qt::Key_E) {
            menuBar()->setActiveAction(ui->menu_edit->menuAction());
        }

    }
}

//拖拽进入事件
void CMyTextEditor::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasUrls())//带有URL
    {
        event->acceptProposedAction();//接收动作
    }
    else
    {
        event->ignore();
    }
}

//拖拽松开事件
void CMyTextEditor::dropEvent(QDropEvent *event)
{
    QWidget *tab_drop = ui->tabWidget->currentWidget();

    const QMimeData *mimeData=event->mimeData();

    if(mimeData->hasUrls())
    {
        QList<QUrl> listUrl=mimeData->urls();

        QString fileName=listUrl[0].toLocalFile();
        if(!fileName.isEmpty())
        {
            QFile file(fileName);
            //打开文件失败
            if(!file.open(QIODevice::ReadOnly)){
                return;
            }

            //打开成功
            QTextStream in(&file);

            if(m_mapWidget2TextEdit_Total[tab_drop]->textEidt->document()->isModified())
            {
                //被修改了要保存
            }

            //新建一个
            QWidget *widget=createTabPage(fileName);
            m_mapWidget2TextEdit_Total[widget]->textEidt->setText(in.readAll());

            int iIndex=ui->tabWidget->currentIndex();
            on_tabWidget_tabCloseRequested(iIndex);//这里必须先新增在删除，如果此时只有一个界面时，不先新增的话无法删除当前界面

            ui->tabWidget->setCurrentWidget(widget);
        }
    }
    else
    {
        event->ignore();
    }
}

//新建
void CMyTextEditor::on_action_New_triggered()
{
    createTabPage();
}

//关闭tabPage
void CMyTextEditor::on_tabWidget_tabCloseRequested(int index)
{
    if(this->iPageCount==1)
    {
        return;
    }

    QWidget *tab_close = ui->tabWidget->widget(index);

    //auto it_New=m_mapWidgt2TextEdit_New.find(tab_close);
    //auto it_File=m_mapWidgt2TextEdit_File.find(tab_close);
    auto it=m_mapWidget2TextEdit_Total.find(tab_close);

    if((*it)->bNew)//找到==是new n类型的界面
    {
        QString name=tab_close->objectName();
        name=name.split("_").at(1);
        bPageNew[std::stoi(name.toStdString())-1]=false;

        m_mapWidget2TextEdit_Total.erase(it);

    }
    else
    {
        if((*it)->textEidt->document()->isModified())
        {
            //另存为
        }
        else
        {
            m_mapWidget2TextEdit_Total.erase(it);
        }
    }


    this->iPageCount--;
    delete tab_close;
}

//打开文件夹
void CMyTextEditor::on_action_Open_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    if(fileName.isEmpty())
    {
        //没有打开
    }
    else
    {
        QFile file(fileName);
        if(file.open(QFile::ReadOnly | QFile::Text))
        {
            QWidget *widget=createTabPage(file.fileName());

            QTextStream in(&file); // 新建文本流对象
            QApplication::setOverrideCursor(Qt::WaitCursor);
            // 读取文件的全部文本内容，并添加到编辑器中
            m_mapWidget2TextEdit_Total[widget]->textEidt->setPlainText(in.readAll());
            QApplication::restoreOverrideCursor();
            ui->tabWidget->setCurrentWidget(widget);
            file.close();
        }
        else
        {
            //打开失败
            QMessageBox::warning(this,"错误",QString("无法读取文件 %1:\n%2.").arg(fileName).arg(file.errorString()));
            return;
        }
    }
}

//保存
void CMyTextEditor::on_action_Save_triggered()
{
    QWidget *tab_save = ui->tabWidget->currentWidget();

    //auto it_New=m_mapWidgt2TextEdit_New.find(tab_save);
    //auto it_File=m_mapWidgt2TextEdit_File.find(tab_save);
    auto it=m_mapWidget2TextEdit_Total.find(tab_save);

    if((*it)->bNew)//新的界面要保存==另存为
    {
        if(m_mapWidget2TextEdit_Total[tab_save]->textEidt->document()->isModified())
        {
            //另存为
            QString fileName = QFileDialog::getSaveFileName(this,"另存为","D:");
            saveAsFile(fileName);
        }
        else
        {
            return;
        }
    }
    else//打开的文件要保存
    {
        if(m_mapWidget2TextEdit_Total[tab_save]->textEidt->document()->isModified())
        {
            //保存
            saveFile(m_mapWidget2TextEdit_Total[tab_save]->filePath);
        }
        else
        {
            return;
        }
    }
}

//另存为
void CMyTextEditor::on_actionl_SaveAs_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this,"另存为","D:");
    saveAsFile(fileName);

}

