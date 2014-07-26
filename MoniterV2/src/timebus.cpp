#include <QTimer>
#include <QTime>
#include "utility/utility.h"
#include "../include/timebus.h"

/*==================================================================
 *    作者：Creater
 *    简介：总线时间
 *    主页： unix8.net
 *    日期：2014-04-22
/*==================================================================*/
long TimeBus::timerCount = 0;

/*!
 * \brief TimeBus::TimeBus 产生一个软件定时器作为心跳产生来源，并获取当前值作为基准时间
 */
TimeBus::TimeBus()
{
    timer = new QTimer();
    QTime currTime = QTime::currentTime();
    int h = currTime.hour();
    int m = currTime.minute();
    int s = currTime.second();
    //修改初始化总线时间
    modifyTimeBusCnt(h * 3600 + m * 60 + s);
    timer->setInterval(1000);
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimerOut()));
    //将启动权已经移交到外部
   // timer->start();
    assistCount = 0;
}

/*!
 * \brief TimeBus::start  外部接口，用来启动该定时器
 */
void TimeBus::start()
{
    timer->start();
}

TimeBus::~TimeBus(){Utility::DESTORY_PTR(timer);}

void TimeBus::incTimeBusCnt()
{
    ++timerCount;
    ++assistCount;
}

long TimeBus::getTimeBusCnt() const
{
    return timerCount;
}

void TimeBus::modifyTimeBusCnt(long tc)
{
    timerCount = tc;
    assistCount = 0;
}

/*!
 * \brief TimeBus::autoModifyTimeBusCnt  由于软件定时不准，所以得每隔5分钟对时间进行一次同步，这是自动调用
 */
void TimeBus::autoModifyTimeBusCnt()
{
    QTime currTime = QTime::currentTime();
    int h = currTime.hour();
    int m = currTime.minute();
    int s = currTime.second();
    modifyTimeBusCnt(h * 3600 + m * 60 + s);
}

/*!
 * \brief TimeBus::onTimerOut  时间临界点判断，自动更新生成,发送信号。
 */
void TimeBus::onTimerOut()
{
    ++timerCount;
    ++assistCount;
    if(timerCount > 86400)
        timerCount = 0;
    //每隔5分钟对时间同步一次
    if(assistCount >= 300)
    {
        autoModifyTimeBusCnt();

    }
    emit sendTime(timerCount);
}
