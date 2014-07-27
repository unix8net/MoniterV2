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

    //���ӵ�������
    void connectToServer(QString& addr, quint16 port);
    //�������ݲɼ���֡ƥ���߳�
    void run();
    //��ʹ�ô���
    void dontUseNet();
	QextSerialPort* getSerialPort(){return serialPort;}
	int getSocketFlag(){return socketFlag;}
	SQLThread& getSqlThread(){return sqlthread;}
private:
	TimeBus timeBus;
	SQLThread sqlthread;
	Producer *dataProducer;
	QextSerialPort* serialPort;

    //����֡ƥ���õ�˫��ѭ������
    CircularDeque<char> circularQueue;
    QMutex mutex;
    QWaitCondition bufferIsEmpty;
    static const int FRAME_LENGTH = 11;
    static const char HEAD_FIRST =  (char)0xfb;
    static const char HEAD_SECOND = (char)0xfd;
    static const char TAIL_FIRST = (char)0xfc;
    static const char TAIL_SECOND = (char)0xfe;

    //�̹߳رձ��
    bool exitFlag;
    /*!
      *�����벼��
      */
    Plot* dataWindow[N_WINDOWS ];
    QGridLayout* gridLayout;


    //sql�߳��뵱ǰ�����ı�
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
    //��������׼����
     void onSerialReadyRead();
     //sqlthread���첽�ص�����������ִ��״̬
     void sqlErrorInfo(const QString &);

     //������زۺ���
     void socketConnected();
     void socketDisconnected();
     void haveError();
     //��������׼����
     void onNetReadyRead();
     void resetConnectServer();
     //����UI
     void updateUI(long);

Q_SIGNALS:
    //���ڽ��յ������ݣ��ַ�����ͬ����ʾ����
     void sendDataToRain(long t, float data);
     void sendDataToShallow(long t, float data);
     void sendDataToDeep(long t, float data);
     void sendDataToAngle(long t, float data);
     void sendDataToStress(long t, float data);
	 void updateLeds();
};



/*!
 * \brief ���ڴ����߳�
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
