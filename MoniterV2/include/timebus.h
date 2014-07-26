#ifndef _TIMEBUS_H_
#define _TIMEBUS_H_
#include <QObject>
/*==================================================================
 *    作者：Creater
 *    简介：作为总线时间，每秒触发一次作为心跳，激发其他功能与界面
 *    主页： unix8.net
 *    日期：2014-04-22
/*==================================================================*/
QT_BEGIN_NAMESPACE
class QTimer;
QT_END_NAMESPACE

class TimeBus : public QObject
{
    Q_OBJECT
public:

    TimeBus();
    ~TimeBus();
    void incTimeBusCnt();
    long getTimeBusCnt() const;
    void modifyTimeBusCnt(long tc);
    void autoModifyTimeBusCnt();
    void start();
private:
    QTimer *timer;
    static long  timerCount;
    long assistCount;

private Q_SLOTS:
     void onTimerOut();
Q_SIGNALS:
     void sendTime(long t);
};


#endif
