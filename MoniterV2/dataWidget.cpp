#include <QGridLayout>
#include <QLabel>
#include <QStatusBar>
#include <QtExtSerialPort/qextserialport.h>
#include "../include/plot.h"
#include "../src/utility/utility.h"
#include "../include/timebus.h"
#include "../include/plot.h"
#include "../include/infowidget.h"
#include "mainwindow.h"
#include "./gomessage.h"
#include "../include/taskthread.h"
#include "dataWidget.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

DataWidget::DataWidget(QWidget *parent):
    QWidget(parent), circularQueue(MAX_SPACE)
{
	setWindowFlags(Qt::Window | Qt::FramelessWindowHint
		| Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint
		| Qt::WindowMaximizeButtonHint);

     QString title[N_WINDOWS] ={
         QString::fromWCharArray(L"����"),
         QString::fromWCharArray(L"ǳ��λ��"),
         QString::fromWCharArray(L"���λ��"),
         QString::fromWCharArray(L"�Ƕ�"),
         QString::fromWCharArray(L"Ӧ��"),
		 QString::fromWCharArray(L"��չ")
     };

    for(int i = 0 ; i < N_WINDOWS; ++i) {
        dataWindow[i] = new Plot(this);
        dataWindow[i]->setYLabel(title[i]);
    }

	setMinimumSize(QSize(100,100));
    gridLayout = new QGridLayout(this);
    gridLayout->addWidget(dataWindow[RAIN_WINDOW], 0, 0);
    gridLayout->addWidget(dataWindow[SHALLOW_WINDOW], 0, 1);
    gridLayout->addWidget(dataWindow[DEEP_WINDOW], 0, 2);
    gridLayout->addWidget(dataWindow[ANGLE_WINDOW], 1, 0);
    gridLayout->addWidget(dataWindow[STRESS_WINDOW], 1, 2);
    gridLayout->addWidget(dataWindow[TEMP], 1, 1);
    gridLayout->setMargin(1);
    gridLayout->setSpacing(1);
    setLayout(gridLayout);

	initPrivateValue();
	dataProducer = new Producer(this);

	//����
	PortSettings settings = {BAUD9600, DATA_8, PAR_NONE, STOP_1, FLOW_OFF, 10};
	serialPort = new QextSerialPort("COM1", settings);
    connect(serialPort, SIGNAL(readyRead()),SLOT(onSerialReadyRead()));
	
    connect(&timeBus, SIGNAL(sendTime(long)), this,SLOT(updateUI(long)) );

	//���յ��������ݺ�ַ����������ν���
    connect(this,SIGNAL(sendDataToRain(long, float)),dataWindow[RAIN_WINDOW],SLOT(setDataByIndex(long, float)));
    connect(this,SIGNAL(sendDataToShallow(long, float)),dataWindow[SHALLOW_WINDOW],SLOT(setDataByIndex(long, float)));
    connect(this,SIGNAL(sendDataToDeep(long, float)),dataWindow[DEEP_WINDOW],SLOT(setDataByIndex(long, float)));
    connect(this,SIGNAL(sendDataToAngle(long, float)),dataWindow[ANGLE_WINDOW],SLOT(setDataByIndex(long, float)));
    connect(this,SIGNAL(sendDataToStress(long, float)), dataWindow[STRESS_WINDOW],SLOT(setDataByIndex(long, float)));

    //����mysql�첽״̬
    connect(&sqlthread,SIGNAL(openError(const QString&)),this,SLOT(sqlErrorInfo(const QString&)));

	//��������״̬������
    connect(&tcpSocket, SIGNAL(connected()), this, SLOT(socketConnected()));
    connect(&tcpSocket, SIGNAL(disconnected()),this, SLOT(socketDisconnected()));
    connect(&tcpSocket, SIGNAL(readyRead()),this, SLOT(onNetReadyRead()));
    connect(&tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(haveError()));

	//����ʱ������
	timeBus.start();
	//�������ݿ�ִ���߳�
	//sqlthread.start();
	dataProducer->start();
}

/*==================================================================    
 *    ���ܣ�   ���º�����ʱ��ֵ��ͬʱ�����ڲ�������
 *    ���������tΪʱ�����
 *    ����ֵ�� 
/*==================================================================*/
void DataWidget::updateUI(long t)
{

    for(int i = 0; i < N_WINDOWS - 1; ++i)
        dataWindow[i]->setIndexByTime(t);

    if(++netCheckCount >= 20) {
        netCheckCount = 0;
        /*!
         * ����ʹ����ClosingState���״̬����ʾϵͳ����ʹ�����磬�����������ڹر�,
         * ��������ԭ������socketFlagʹ��QAbstractSocket��ͳһ��������
        */
        if(socketFlag == QAbstractSocket ::UnconnectedState) {
            QTimer::singleShot(0, this, SLOT(resetConnectServer()));
        }
    }
}

/*==================================================================    
 *    ���ܣ�   ��ʼ��һЩֵ��exitFlagΪ�����˳���־
 *    ���������
 *    ����ֵ�� 
/*==================================================================*/
void DataWidget::initPrivateValue()
{
    exitFlag = false;
    socketFlag = QAbstractSocket::ClosingState;
    netCheckCount = 0;
}

/*==================================================================    
 *    ���ܣ�   ÿ��20S���һ���Ƿ���Ҫ���ӵ����磬���ʹ�ô�������Ҫ
 *    ���������
 *    ����ֵ�� 
/*==================================================================*/
void DataWidget::resetConnectServer()
{
    if(tcpSocket.state() == QAbstractSocket::UnconnectedState){
        tcpSocket.close();
        connectToServer(serverIp, serverPort);
    }
}

/*==================================================================    
 *    ���ܣ�   ���ӵ�������
 *    ���������
 *    ����ֵ�� 
/*==================================================================*/
void DataWidget::connectToServer(QString &addr, quint16 port)
{
    serverIp = addr;
    serverPort = port;
    socketFlag = QAbstractSocket::UnconnectedState;
    tcpSocket.connectToHost(addr, port);
}

/*==================================================================    
 *    ���ܣ�   ���ӳɹ�����������ص�  --1
 *    ���������
 *    ����ֵ�� 
/*==================================================================*/
void DataWidget::socketConnected()
{
    socketFlag = QAbstractSocket ::ConnectedState;
    goMessage * mess = new goMessage(QString::fromWCharArray(L"���ӷ������˳ɹ�!"), 2000, this);
    mess->show();
  //  led[TCPSERVER_LED]->turnOn();
}


/*==================================================================    
 *    ���ܣ�   �Ͽ������������Ӻ�Ļص� ---2
 *    ���������
 *    ����ֵ�� 
/*==================================================================*/
void DataWidget::socketDisconnected()
{
    socketFlag = QAbstractSocket::UnconnectedState;
    goMessage * mess = new goMessage(QString::fromWCharArray(L"�������Ͽ����ӣ�ϵͳ������TCP����!"), 2000, this);
    mess->show();
   // led[TCPSERVER_LED]->turnOff();
    tcpSocket.close();
}

/*==================================================================    
 *    ���ܣ�  �����쳣������ 
 *    ���������
 *    ����ֵ�� 
/*==================================================================*/
void DataWidget::haveError()
{
    socketFlag = QAbstractSocket::UnconnectedState;
    goMessage * mess = new goMessage(QString::fromWCharArray(L"�����쳣�����ܽ��յ���������!"), 3000, this);
    mess->show();
 //   led[TCPSERVER_LED]->haveError();
    tcpSocket.close();
}

/*==================================================================    
 *    ���ܣ�   ��������׼����
 *    ���������
 *    ����ֵ�� 
/*==================================================================*/
void DataWidget::onNetReadyRead()
{
    if (tcpSocket.bytesAvailable() >= 2){
        //if(mutex.tryLock())
        {
            mutex.lock();
            QByteArray qbyte = tcpSocket.readAll();
            circularQueue.push_n_back(qbyte.data(), qbyte.size());
             mutex.unlock();
             bufferIsEmpty.wakeAll();
        }
    }
}

/*==================================================================    
 *    ���ܣ�   ��������׼����
 *    ���������
 *    ����ֵ�� 
/*==================================================================*/
void DataWidget::onSerialReadyRead()
{
    if (serialPort->bytesAvailable() >= 2){
      QByteArray qbyte = serialPort->readAll();
      mutex.lock();
      circularQueue.push_n_back(qbyte.data(), qbyte.size());
      mutex.unlock();
      bufferIsEmpty.wakeAll();
    }
}

DataWidget::~DataWidget()
{
    //ok call
	exitFlag = true;
	bufferIsEmpty.wakeAll();
	dataProducer->waitThread();
}

/*!
 * \brief �첽���ݿ��̳߳���ص�����
 * \param s
 */
void DataWidget::sqlErrorInfo(const QString &)
{
    //led[MYSQL_LED]->turnOff();
}

void DataWidget::closeEvent(QCloseEvent *)
{
    exitFlag = true;
    bufferIsEmpty.wakeAll();
	dataProducer->waitThread();
}


/*!
 * \brief ������Producer�в������߳���ִ��
 */
void DataWidget::run()
{
    bool bstart = false;
    int cnt = 0;
    char tmp[FRAME_LENGTH - 1];
    char buff[2];
    int len;
    long oldTime = -1;
    float data ;
    while(true)
    {
        mutex.lock();
        while(circularQueue.size() <= 2)
        {
           bufferIsEmpty.wait(&mutex);
           if(exitFlag)
           {
               mutex.unlock();
               return;
           }
        }
        if(!bstart)
        {
            while(true)
            {
                if(circularQueue.size() < 2) continue;
                circularQueue.get_n_front(buff, 2);
                if(!(buff[0] ^ HEAD_FIRST) && !(buff[1] ^ HEAD_SECOND))
                {
                    bstart = true;
                    cnt = 0;
                    circularQueue.remove_n_front(2);
                    len = circularQueue.front();
                    break;
                }
                else
                {
                    circularQueue.remove_n_front(1);
                }
            }
        }
        else
        {
            while(true)
            {
                if(circularQueue.size() < 2) break;
                circularQueue.get_n_front(buff,2);
                if(!(buff[0] ^ TAIL_FIRST) && !(buff[1] ^ TAIL_SECOND))
                {
                    memcpy(&data, tmp + 3, sizeof(float));
                    long nowTime = timeBus.getTimeBusCnt();
                    QDateTime time = QDateTime::currentDateTime();

                    if((oldTime > nowTime)
                            ||(oldTime == -1))
                    {
                        QString str = time.toString("yyyyMMdd");
                        rainTable = str + "rn";
                        shallowTable = str + "sh";
                        deepTable = str + "dp";
                        angleTable = str + "ag";
                        stressTable = str + "ss";
                        sqlthread.asyncCreatTable(rainTable);//rain
                        sqlthread.asyncCreatTable(shallowTable);//shallow
                        sqlthread.asyncCreatTable(deepTable);//deep
                        sqlthread.asyncCreatTable(angleTable);//angle
                        sqlthread.asyncCreatTable(stressTable);//stress
                    }
                    oldTime = nowTime;
                    QString strAll = time.toString("yyyy-MM-dd hh:mm:ss");
                    switch ( *(tmp + 1) )
                    {
                    /*!
                      * ����Ϊ�����ݷַ�������������ʾģ�飬����ʹ���첽��ʽ���뵽���ݿ���
                      */

					case DataWindows::RAIN_WINDOW:{
                        emit sendDataToRain(nowTime, data);                 
                        sqlthread.asyncInsertData(rainTable ,strAll, nowTime, data );
                        break;}
                    case DataWindows::SHALLOW_WINDOW:{
                        emit sendDataToShallow(nowTime, data);                     
                        sqlthread.asyncInsertData(shallowTable ,strAll, nowTime, data );
                        break;}
                    case DataWindows::DEEP_WINDOW:{
                        emit sendDataToDeep(nowTime, data);
                        sqlthread.asyncInsertData(deepTable ,strAll, nowTime, data );
                        break;}

                    case DataWindows::ANGLE_WINDOW:{
                        emit sendDataToAngle(nowTime, data);
                        sqlthread.asyncInsertData(angleTable ,strAll, nowTime, data );
                        break;}
                    case DataWindows::STRESS_WINDOW:{
                        emit sendDataToStress(nowTime, data);
                        sqlthread.asyncInsertData(stressTable ,strAll, nowTime, data );
                        break;}
                        default:break;
                    }
                    //qDebug()<<"�յ�����������֡:"<<cnt;
                    bstart = false;
                    cnt = 0;
                    circularQueue.remove_n_front(2);
                    break;
                }
                else
                {
                    circularQueue.remove_n_front(1);
                    tmp[cnt++] = buff[0];
                    if(cnt >= FRAME_LENGTH -1)
                    {
                        bstart = false;
                        cnt = 0;
                    }
                }

            }
        }
        mutex.unlock();

    }
}

/*!
 * \brief �����ô���ʱ���ͷ�һЩconnect
 */
void DataWidget::dontUseNet()
{
    tcpSocket.close();
    disconnect(&tcpSocket, SIGNAL(connected()), this, SLOT(socketConnected()));
    disconnect(&tcpSocket, SIGNAL(disconnected()),this, SLOT(socketDisconnected()));
    disconnect(&tcpSocket, SIGNAL(readyRead()),this, SLOT(onNetReadyRead()));
    disconnect(&tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(haveError()));
}


Producer::Producer(QObject *parent) 
	: QThread(parent)
{
}


void Producer::run()
{
	static_cast<DataWidget*>(parent())->run();
}
void Producer::waitThread()
{
	// ok call
	wait();
}

Producer::~Producer()
{
    waitThread();
    exit(0);
}
