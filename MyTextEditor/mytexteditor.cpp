#include "mytexteditor.h"
#include "ui_mytexteditor.h"

CMyTextEditor::CMyTextEditor(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CMyTextEditor)
{
    ui->setupUi(this);
    setAcceptDrops(true);

    this->fontDialog=new QFontDialog;
    this->colorDialog_font=new QColorDialog;
    this->colorDialog_fontBg=new QColorDialog;

    //创建一个字体json文件
    QFile file(strFileJson);

    if (!file.exists())
    {
        // 文件不存在，创建新的空文件
        if (!file.open(QIODevice::WriteOnly))
        {
            qDebug() << "无法创建新文件：" << strFileJson;
            return;
        }
        file.close();
    }

    //设置上下文菜单策略
    setContextMenuPolicy(Qt::DefaultContextMenu);//使用Qt::ActionsContextMenu的方式会失效

    //查找框
    this->DlgFind=new QDialog(this);
    this->DlgFind->setWindowTitle("查找");
    this->leFind=new QLineEdit(this->DlgFind);
    this->lblFind=new QLabel(this->DlgFind);
    this->lblFind->setText("");
    QPushButton *btnBackward=new QPushButton("向后查找",this->DlgFind);
    QPushButton *btnForward=new QPushButton("向前查找",this->DlgFind);
    QVBoxLayout *vLayout=new QVBoxLayout(this->DlgFind);
    vLayout->addWidget(this->leFind);
    vLayout->addWidget(this->lblFind);

    QHBoxLayout *hLayout=new QHBoxLayout(this->DlgFind);
    hLayout->addWidget(btnBackward);
    hLayout->addWidget(btnForward);
    vLayout->addLayout(hLayout);

    connect(btnBackward,&QPushButton::clicked,this,&CMyTextEditor::findTextBackward);
    connect(btnForward,&QPushButton::clicked,this,&CMyTextEditor::findTextForward);

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
            loadFileFont(widget);
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

    if((*it)->textEidt->document()->isModified())
    {
        if((*it)->bNew)//找到==是new n类型的界面
        {
            //另存为
            QString fileName = QFileDialog::getSaveFileName(this,"另存为","D:");
            saveAsFile(fileName);

            QString name=tab_close->objectName();
            name=name.split("_").at(1);
            bPageNew[std::stoi(name.toStdString())-1]=false;

            m_mapWidget2TextEdit_Total.erase(it);

        }
        else
        {
            bool bSave=saveFile((*it)->filePath);
            if(bSave)
            {
                QMessageBox::warning(this,"提示",QString("文件保存到%1").arg((*it)->filePath));
                saveFileFont(tab_close);
                m_mapWidget2TextEdit_Total.erase(it);
            }
        }
    }
    else
    {
        //没改过就不做任何处理
        qDebug()<<"没有修改文件";
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
            loadFileFont(widget);

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

//复制
void CMyTextEditor::on_action_Copy_triggered()
{
    QWidget *tab_save = ui->tabWidget->currentWidget();
    m_mapWidget2TextEdit_Total[tab_save]->textEidt->copy();
}

//粘贴
void CMyTextEditor::on_action_Paste_triggered()
{
    QWidget *tab_save = ui->tabWidget->currentWidget();
    m_mapWidget2TextEdit_Total[tab_save]->textEidt->paste();
}

//撤销
void CMyTextEditor::on_action_Undo_triggered()
{
    QWidget *tab_save = ui->tabWidget->currentWidget();
    m_mapWidget2TextEdit_Total[tab_save]->textEidt->undo();
}

//剪切
void CMyTextEditor::on_action_Cut_triggered()
{
    QWidget *tab_save = ui->tabWidget->currentWidget();
    m_mapWidget2TextEdit_Total[tab_save]->textEidt->cut();
}

//查找
void CMyTextEditor::on_action_Find_triggered()
{
    this->DlgFind->show();
}

//向后查找
void CMyTextEditor::findTextBackward()
{
    QWidget *tab_save = ui->tabWidget->currentWidget();
    QString str = this->leFind->text();

    if(m_mapWidget2TextEdit_Total[tab_save]->textEidt->toPlainText().contains(str))
    {
        this->lblFind->setText("");
        if (!m_mapWidget2TextEdit_Total[tab_save]->textEidt->find(str, QTextDocument::FindBackward))
        {
            this->lblFind->setText("到达文件头");
            m_mapWidget2TextEdit_Total[tab_save]->textEidt->moveCursor(QTextCursor::End);
        }
    }
    else
    {
        QMessageBox::warning(this->DlgFind,"查找",QString("找不到%1").arg(str));
    }
}

//向前查找
void CMyTextEditor::findTextForward()
{
    QWidget *tab_save = ui->tabWidget->currentWidget();
    QString str = this->leFind->text();

    if (m_mapWidget2TextEdit_Total[tab_save]->textEidt->toPlainText().contains(str))
    {
        this->lblFind->setText("");
        if (!m_mapWidget2TextEdit_Total[tab_save]->textEidt->find(str))
        {
            this->lblFind->setText("到达文件尾");
            m_mapWidget2TextEdit_Total[tab_save]->textEidt->moveCursor(QTextCursor::Start);
        }
    }
    else
    {
        QMessageBox::warning(this->DlgFind, "查找", QString("找不到%1").arg(str));
    }
}

//保存字体信息到文件
//输入参数应该是完整的路径
void CMyTextEditor::saveFileFont(QWidget *tab_cur)
{
    if(m_mapWidget2TextEdit_Total.find(tab_cur)==m_mapWidget2TextEdit_Total.end())
    {
        return;//没找到
    }
    QString fileName=m_mapWidget2TextEdit_Total[tab_cur]->filePath;

    QTextCursor cursor=m_mapWidget2TextEdit_Total[tab_cur]->textEidt->textCursor();
    QFont font=cursor.charFormat().font();

    QJsonObject fontAttributes;//json块
    fontAttributes[FilePath] = fileName;
    fontAttributes[FontFamily] = font.family();
    fontAttributes[FontSize] = font.pointSize();
    fontAttributes[FontWeight] = font.weight();
    fontAttributes[Underline] = font.underline();
    fontAttributes[Strikeout] = font.strikeOut();
    fontAttributes[Italic] = font.italic();

    QFile file(strFileJson);
    QJsonDocument jsonDoc;

    if(file.exists() && file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        QString content=in.readAll();
        file.close();

        jsonDoc=QJsonDocument::fromJson(content.toUtf8());
    }
    else
    {
        //不存在
        qDebug()<<"找不到Json文件："<<strFileJson;
        return;
    }

    QJsonArray jsonArray=jsonDoc.array();

    bool bFindFilePath=false;//是否已经存在该文件字段
    for (int i = 0; i < jsonArray.size(); ++i)
    {
        QJsonObject obj = jsonArray.at(i).toObject();
        if (obj[FilePath].toString() == fontAttributes[FilePath].toString())
        {
            // 更新已存在的块
            obj = fontAttributes;
            jsonArray.replace(i, obj);
            bFindFilePath = true;
            break;
        }
    }

    // 如果不存在，添加新块
    if (!bFindFilePath)
    {
        jsonArray.append(fontAttributes);
    }

    jsonDoc.setArray(jsonArray);

    // 保存 JSON 到文件
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file);
        out << jsonDoc.toJson();
        file.close();
    }
}

//加载字体信息到
void CMyTextEditor::loadFileFont(QWidget *widget)
{
    if(m_mapWidget2TextEdit_Total.find(widget)==m_mapWidget2TextEdit_Total.end())
    {
        return;//没找到
    }
    QString fileName=m_mapWidget2TextEdit_Total[widget]->filePath;

    QFile file(strFileJson);
    if (!file.exists() || !file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        // 文件不存在或无法打开
        return;
    }

    QTextStream in(&file);
    QString content = in.readAll();
    file.close();

    QJsonDocument jsonDoc = QJsonDocument::fromJson(content.toUtf8());

    // 获取 JSON 数组
    QJsonArray jsonArray = jsonDoc.array();

    // 查找是否存在相同的 filePath
    for (int i = 0; i < jsonArray.size(); ++i)
    {
        QJsonObject obj = jsonArray.at(i).toObject();
        if (obj[FilePath].toString() == fileName)
        {
            // 使用 JSON 中的字体信息更新当前 widget
            setFontFamily(obj[FontFamily].toString(), widget,Opt_All);
            setFontSize(obj[FontSize].toInt(), widget,Opt_All);
            setFontWeight(obj[FontWeight].toInt(), widget,Opt_All);
            setUnderline(obj[Underline].toBool(), widget,Opt_All);
            setStrikeout(obj[Strikeout].toBool(), widget,Opt_All);
            setItalic(obj[Italic].toBool(), widget,Opt_All);
            break;
        }
    }
}

//设置字体类型
void CMyTextEditor::setFontFamily(QString strFontFamily,QWidget *widget,int iType)
{
    QWidget *tab_cur=nullptr;
    if(widget==nullptr)
    {
        tab_cur = ui->tabWidget->currentWidget();
    }
    else
    {
        tab_cur = widget;
    }

    if(iType==Opt_Selected)//设置选中的文本
    {
        QTextCursor cursor=m_mapWidget2TextEdit_Total[tab_cur]->textEidt->textCursor();
        QTextCharFormat format = cursor.charFormat();

        // 设置字体
        format.setFont(strFontFamily);

        // 应用字体属性到当前选中的文本
        cursor.mergeCharFormat(format);
    }
    else
    {
        QTextDocument *document=m_mapWidget2TextEdit_Total[tab_cur]->textEidt->document();

        QTextCharFormat format;
        format.setFont(strFontFamily);
        document->setDefaultFont(format.font());
    }

}

//设置字体大小
void CMyTextEditor::setFontSize(int iFontSize,QWidget *widget,int iType)
{
    QWidget *tab_cur=nullptr;
    if(widget==nullptr)
    {
        tab_cur = ui->tabWidget->currentWidget();
    }
    else
    {
        tab_cur = widget;
    }

    if(iType==Opt_Selected)//设置选中的文本
    {
        QTextCursor cursor=m_mapWidget2TextEdit_Total[tab_cur]->textEidt->textCursor();
        QTextCharFormat format = cursor.charFormat();

        // 设置字体大小
        format.setFontPointSize(iFontSize);


        // 应用字体属性到当前选中的文本
        cursor.mergeCharFormat(format);
    }
    else
    {
        QTextDocument *document=m_mapWidget2TextEdit_Total[tab_cur]->textEidt->document();

        QTextCharFormat format;
        format.setFontPointSize(iFontSize);
        document->setDefaultFont(format.font());
    }
}


//设置字体粗细
void CMyTextEditor::setFontWeight(int iWeight,QWidget *widget,int iType)
{
    QWidget *tab_cur=nullptr;
    if(widget==nullptr)
    {
        tab_cur = ui->tabWidget->currentWidget();
    }
    else
    {
        tab_cur = widget;
    }

    if(iType==Opt_Selected)//设置选中的文本
    {
        QTextCursor cursor=m_mapWidget2TextEdit_Total[tab_cur]->textEidt->textCursor();
        QTextCharFormat format = cursor.charFormat();

        //设置字体粗细
        format.setFontWeight(iWeight);

        // 应用字体属性到当前选中的文本
        cursor.mergeCharFormat(format);
    }
    else//设置全体文本
    {
        QTextDocument *document=m_mapWidget2TextEdit_Total[tab_cur]->textEidt->document();

        QFont defaultFont = document->defaultFont();
        QFont::Weight weight = static_cast<QFont::Weight>(iWeight);
        defaultFont.setWeight(weight);

        QTextCharFormat format;
        format.setFont(defaultFont);

        document->setDefaultFont(defaultFont);


    }
}

//设置下划线
void CMyTextEditor::setUnderline(bool bUnderline,QWidget *widget,int iType)
{
    QWidget *tab_cur=nullptr;
    if(widget==nullptr)
    {
        tab_cur = ui->tabWidget->currentWidget();
    }
    else
    {
        tab_cur = widget;
    }

    if(iType==Opt_Selected)//设置选中的文本
    {
        QTextCursor cursor=m_mapWidget2TextEdit_Total[tab_cur]->textEidt->textCursor();
        QTextCharFormat format = cursor.charFormat();

        //设置下划线
        format.setFontUnderline(bUnderline);

        // 应用字体属性到当前选中的文本
        cursor.mergeCharFormat(format);
    }
    else//设置整个文本
    {
        QTextDocument *document=m_mapWidget2TextEdit_Total[tab_cur]->textEidt->document();

        QFont defaultFont = document->defaultFont();
        defaultFont.setUnderline(bUnderline);

        QTextCharFormat format;
        format.setFont(defaultFont);

        document->setDefaultFont(defaultFont);

    }
}

//设置删除线
void CMyTextEditor::setStrikeout(bool bStrikeout,QWidget *widget,int iType)
{
    QWidget *tab_cur=nullptr;
    if(widget==nullptr)
    {
        tab_cur = ui->tabWidget->currentWidget();
    }
    else
    {
        tab_cur = widget;
    }

    if(iType==Opt_Selected)//设置选中的文本
    {
        QTextCursor cursor=m_mapWidget2TextEdit_Total[tab_cur]->textEidt->textCursor();
        QTextCharFormat format = cursor.charFormat();

        //设置删除线
        format.setFontStrikeOut(bStrikeout);

        // 应用字体属性到当前选中的文本
        cursor.mergeCharFormat(format);
    }
    else
    {
        QTextDocument *document=m_mapWidget2TextEdit_Total[tab_cur]->textEidt->document();

        QFont defaultFont = document->defaultFont();
        defaultFont.setStrikeOut(bStrikeout);

        QTextCharFormat format;
        format.setFont(defaultFont);

        document->setDefaultFont(defaultFont);
    }
}

//字体倾斜
void CMyTextEditor::setItalic(bool bItalic, QWidget *widget,int iType)
{
    QWidget *tab_cur=nullptr;
    if(widget==nullptr)
    {
        tab_cur = ui->tabWidget->currentWidget();
    }
    else
    {
        tab_cur = widget;
    }

    if(iType==Opt_Selected)//设置选中的文本
    {
        QTextCursor cursor=m_mapWidget2TextEdit_Total[tab_cur]->textEidt->textCursor();
        QTextCharFormat format = cursor.charFormat();

        //设置删除线
        format.setFontItalic(bItalic);

        // 应用字体属性到当前选中的文本
        cursor.mergeCharFormat(format);
    }
    else
    {
        QTextDocument *document=m_mapWidget2TextEdit_Total[tab_cur]->textEidt->document();

        QFont defaultFont = document->defaultFont();
        defaultFont.setItalic(bItalic);

        QTextCharFormat format;
        format.setFont(defaultFont);

        document->setDefaultFont(defaultFont);

    }

}

//字体加粗
void CMyTextEditor::on_action_TextBold_triggered()
{
    QWidget *tab_cur = ui->tabWidget->currentWidget();
    QTextCursor cursor=m_mapWidget2TextEdit_Total[tab_cur]->textEidt->textCursor();
    QTextCharFormat format = cursor.charFormat();

    format.setFontWeight((format.fontWeight() == QFont::Black) ? QFont::Normal : QFont::Black);


    // 应用字体属性到当前选中的文本
    cursor.mergeCharFormat(format);
}

//字体
void CMyTextEditor::on_action_TextType_triggered()
{
    //先将当前字体信息设置进去
    QWidget *tab_cur = ui->tabWidget->currentWidget();
    QTextCursor cursor=m_mapWidget2TextEdit_Total[tab_cur]->textEidt->textCursor();
    QFont font=cursor.charFormat().font();
    this->fontDialog->setCurrentFont(font);

    if (this->fontDialog->exec() == QDialog::Accepted)
    {
        QFont selectedFont = this->fontDialog->selectedFont();
        //qDebug()<<selectedFont;

        QString fontFamily = selectedFont.family();  // 获取字体类型
        setFontFamily(fontFamily);
        int fontSize = selectedFont.pointSize();     // 获取字体大小
        setFontSize(fontSize);
        int fontWeight = selectedFont.weight();      // 获取字体粗细
        setFontWeight(fontWeight);
        bool underline = selectedFont.underline();   // 获取是否有下划线
        setUnderline(underline);
        bool strikeout = selectedFont.strikeOut();   // 获取是否有删除线
        setStrikeout(strikeout);

    }

}

//字体倾斜
void CMyTextEditor::on_action_TextItalic_triggered()
{
    QWidget *tab_cur = ui->tabWidget->currentWidget();
    QTextCursor cursor=m_mapWidget2TextEdit_Total[tab_cur]->textEidt->textCursor();
    QTextCharFormat format = cursor.charFormat();

    format.setFontItalic(!format.fontItalic());

    // 应用字体属性到当前选中的文本
    cursor.mergeCharFormat(format);
}

//删除线
void CMyTextEditor::on_action_TextStrike_triggered()
{
    QWidget *tab_cur = ui->tabWidget->currentWidget();
    QTextCursor cursor=m_mapWidget2TextEdit_Total[tab_cur]->textEidt->textCursor();
    QTextCharFormat format = cursor.charFormat();

    format.setFontStrikeOut(!format.fontStrikeOut());

    // 应用字体属性到当前选中的文本
    cursor.mergeCharFormat(format);
}

//下划线
void CMyTextEditor::on_action_TextUnder_triggered()
{
    QWidget *tab_cur = ui->tabWidget->currentWidget();
    QTextCursor cursor=m_mapWidget2TextEdit_Total[tab_cur]->textEidt->textCursor();
    QTextCharFormat format = cursor.charFormat();

    format.setFontUnderline(!format.fontUnderline());

    // 应用字体属性到当前选中的文本
    cursor.mergeCharFormat(format);
}

//关于
void CMyTextEditor::on_action_About_triggered()
{
    QWidget *tab_cur = ui->tabWidget->currentWidget();
    loadFileFont(tab_cur);
}

//字体颜色
void CMyTextEditor::on_action_color_triggered()
{
    QColor color= this->colorDialog_font->getColor(Qt::black,this,"字体颜色");

    if(color.isValid())
    {
        QWidget *tab_cur = ui->tabWidget->currentWidget();
        QTextCursor cursor=m_mapWidget2TextEdit_Total[tab_cur]->textEidt->textCursor();
        QTextCharFormat format = cursor.charFormat();

        format.setForeground(color);

        // 应用字体属性到当前选中的文本
        cursor.mergeCharFormat(format);

    }
}

//字体背景颜色
void CMyTextEditor::on_action_bgColor_triggered()
{
    QColor color = this->colorDialog_fontBg->getColor(Qt::white, this, "字体背景颜色");

    if (color.isValid())
    {
        QWidget *tab_cur = ui->tabWidget->currentWidget();
        QTextCursor cursor = m_mapWidget2TextEdit_Total[tab_cur]->textEidt->textCursor();
        QTextCharFormat format = cursor.charFormat();

        // 设置字体背景颜色
        format.setBackground(QBrush(color));
        cursor.setCharFormat(format);  // 设置新的背景颜色
    }
}

