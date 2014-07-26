#ifndef TASKTHREAD_H
#define TASKTHREAD_H



#include <QThread>
#include <QSemaphore>

#include "../include/sql.h"

class SQLThread : public QThread
{
    Q_OBJECT

public:
    enum SQLTasks
    {
        //�������ݿ�
        ConnectDb,
        //������
        CreatTable,
        //��������
        InsertData,
        //��ѯ����
        SelectData,
        NTasks //4
    };

    //����ʹ�����ݶ���
    struct SQLDataRarelyUsed
    {
        /*!
         *�ù��캯��Ĭ��Ϊ����ָ����߲�ѯָ���t����
         */
        SQLDataRarelyUsed(QString table_name, SQLTasks t) : name(table_name), task(t)
        {
        }

        /*!
         *�ù��캯��Ĭ��Ϊ�������ݿ�ָ��
         */
        SQLDataRarelyUsed(const QString& db_name, const QString& user_,
			const  QString& pass_, SQLTasks t) :
            name(db_name), user(user_), passwd(pass_), task(t)
        {
        }

        /*!
         * �����������ݿ��ã����ڽ����ã����ڲ�ѯ����
         */
        const QString name; //db_name,table_name
        /*!
         * �����������ݿ���
         */
        const QString user;
        const QString passwd;

        SQLThread::SQLTasks task;
    };

    //��ʹ�õĲ���ָ�����
    struct SQLDataFrequentUsed
    {
        /*!
         * �����������ָ��
         */
        SQLDataFrequentUsed(const QString& table_name, const QString& t, long id_, float data_) :
            table(table_name), time(t), id(id_), data(data_){}
        const QString table;
        const QString time;
        long id;
        float data;
    };

    SQLThread();
    virtual ~SQLThread();
    void run();

    //�ⲿ���ã����ӵ����ݿ�
    void asyncConnectDb(const QString&, const QString&, const QString&);
    //�ⲿ���ã��������ݱ�
    void asyncCreatTable(const QString &);
    //�ⲿ����,����в�������
    void asyncInsertData(const QString&, const QString&, long, float);
    //�ⲿ���ã���ѯ��
    void asyncSelectTable(const QString&);

private:
    SQL sql;
    QSemaphore freeSemaphore;
    QSemaphore usedSemaphore;
    QList<SQLDataRarelyUsed> rarelyUsed;
    QList<SQLDataFrequentUsed> frequentUsed;

    bool exitFlag;

Q_SIGNALS:
    //�첽��Ϣ�ַ�
     void openError(const QString&);
     //�첽��ȡ��ѯ��Ľ��
     void getResult(MyVector&, int);
};
#endif