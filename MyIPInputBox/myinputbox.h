#ifndef MYINPUTBOX_H
#define MYINPUTBOX_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QEvent>
#include <QKeyEvent>
#include <QString>
#include <QStringList>
#include <QVBoxLayout>

class MyInputBox : public QWidget
{
    Q_OBJECT
public:
    explicit MyInputBox(QWidget *parent = nullptr);
    void clear();
    void setIPAddress(QString ip);

private:
    QVector<QLabel*> m_vecPoint;//小圆点集合

    QVector<QLineEdit*> m_vecIP;//IP地址输入框集合

    QString ip;
    QColor bgColor;
    QColor borderColor;
    int borderRadius;//边框圆角半径

private slots:
    void textChanged(const QString &text);


protected:
    bool eventFilter(QObject *watched, QEvent *event);


};

#endif // MYINPUTBOX_H
