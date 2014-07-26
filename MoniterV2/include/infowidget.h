#ifndef _INFOWIDGET_H_
#define _INFOWIDGET_H_
/*==================================================================
 *    作者：Creater
 *    简介：该类用于在主面板中显示系统相关信息,包括系统时间，数据库状态，串口状态
 *    主页： unix8.net
 *    日期：2014-04-22 ,校正2014-05-03
/*==================================================================*/
#include <QWidget>
QT_BEGIN_NAMESPACE
class QGridLayout;
class QLabel;
QT_END_NAMESPACE

class InfoWidget: public QWidget
{
    Q_OBJECT

public:
    InfoWidget( QWidget * = 0 );
    ~InfoWidget();
    void setSqlText(QString);
    void setSqlTextPen(QColor color);
private:
    QGridLayout* gLayout;
    QLabel* runtimeLabel;
    //QLabel* sqlState;
    //QLabel* threadState;

public Q_SLOTS:
    void timeBusDispath(long t);
};

#endif
