#ifndef GOMESSAGE_H
#define GOMESSAGE_H

#include <QDialog>
#include <QString>
/*==================================================================
 *    作者：Creater
 *    简介：信息提示倒计时界面
 *    主页： unix8.net
 *    日期：2014-04-24
/*==================================================================*/
namespace Ui {
class goMessage;
}

class goMessage : public QDialog
{
    Q_OBJECT

public:
    explicit goMessage(const QString &mess, int t = 3000, QWidget *parent = 0);
    ~goMessage();

private:
    Ui::goMessage *ui;
private Q_SLOTS:
     void timeOver();
};

#endif // GOMESSAGE_H
