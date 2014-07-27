#ifndef DATAWIDGET_H
#define DATAWIDGET_H

#include <QWidget>
#include <QWaitCondition>
#include <QMutex>
#include <QThread>
#include <QTcpSocket>
#include "circularQueue/circular_deque.h"
#include "../include/timebus.h"
#include "../include/taskthread.h"
#include <QtExtSerialPort/qextserialport.h>

QT_BEGIN_NAMESPACE
class TimeBus;
class SpecialBaseWidget;
class QGridLayout;
class QextSerialPort;
class InfoWidget;
class SQLThread;
class QLabel;
class HLed;
class HLeds;
class Plot;
class Producer;

QT_END_NAMESPACE

#define MAX_SPACE 256


class DataWidget : public QWidget
{
    Q_OBJECT

public:

    enum DataWindows{
        RAIN_WINDOW = 0x01,
        SHALLOW_WINDOW = 0x02,
        DEEP_WINDOW = 0x03,
        ANGLE_WINDOW = 0x04,
        STRESS_WINDOW = 0x05,
		TEMP = 0x00,
        N_WINDOWS = 0x06
    };

    DataWidget(QWidget *parent = 0);
    ~DataWidget();
    void closeEvent(QCloseEvent *e);

    //连接到服务器
    void connectToServer(QString& addr, quint16 port);
    //运行数据采集与帧匹配线程
    void run();
    //不使用串口
    void dontUseNet();
	QextSerialPort* getSerialPort(){return serialPort;}
	int getSocketFlag(){return socketFlag;}
	SQLThread& getSqlThread(){return sqlthread;}
private:
	TimeBus timeBus;
	SQLThread sqlthread;
	Producer *dataProducer;
	QextSerialPort* serialPort;

    //串口帧匹配用的双端循环队列
    CircularDeque<char> circularQueue;
    QMutex mutex;
    QWaitCondition bufferIsEmpty;
    static const int FRAME_LENGTH = 11;
    static const char HEAD_FIRST =  (char)0xfb;
    static const char HEAD_SECOND = (char)0xfd;
    static const char TAIL_FIRST = (char)0xfc;
    static const char TAIL_SECOND = (char)0xfe;

    //线程关闭标记
    bool exitFlag;
    /*!
      *界面与布局
      */
    Plot* dataWindow[N_WINDOWS ];
    QGridLayout* gridLayout;


    //sql线程与当前操作的表
    QString rainTable;
    QString shallowTable;
    QString deepTable;
    QString angleTable;
    QString stressTable;


    QTcpSocket tcpSocket;
    int socketFlag;
    QString serverIp;
    quint16 serverPort;
    int netCheckCount;

    void initPrivateValue();

private Q_SLOTS:
    //串口数据准备好
     void onSerialReadyRead();
     //sqlthread的异步回调函数，返回执行状态
     void sqlErrorInfo(const QString &);

     //网络相关槽函数
     void socketConnected();
     void socketDisconnected();
     void haveError();
     //网络数据准备好
     void onNetReadyRead();
     void resetConnectServer();
     //更新UI
     void updateUI(long);

Q_SIGNALS:
    //用于将收到的数据，分发给不同的显示界面
     void sendDataToRain(long t, float data);
     void sendDataToShallow(long t, float data);
     void sendDataToDeep(long t, float data);
     void sendDataToAngle(long t, float data);
     void sendDataToStress(long t, float data);
	 void updateLeds();
};



/*!
 * \brief 串口处理线程
 */
class Producer : public QThread
{
    Q_OBJECT
public:
    Producer(QObject * parent = 0);
    virtual ~Producer();
    void run();
	void waitThread();
};


#endif // MAINWINDOW_H
