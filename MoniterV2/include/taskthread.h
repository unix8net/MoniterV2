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
        //连接数据库
        ConnectDb,
        //创建表
        CreatTable,
        //插入数据
        InsertData,
        //查询数据
        SelectData,
        NTasks //4
    };

    //很少使用数据队列
    struct SQLDataRarelyUsed
    {
        /*!
         *该构造函数默认为建表指令或者查询指令，由t区分
         */
        SQLDataRarelyUsed(QString table_name, SQLTasks t) : name(table_name), task(t)
        {
        }

        /*!
         *该构造函数默认为连接数据库指令
         */
        SQLDataRarelyUsed(const QString& db_name, const QString& user_,
			const  QString& pass_, SQLTasks t) :
            name(db_name), user(user_), passwd(pass_), task(t)
        {
        }

        /*!
         * 用于连接数据库用，用于建表用，用于查询表用
         */
        const QString name; //db_name,table_name
        /*!
         * 用于连接数据库用
         */
        const QString user;
        const QString passwd;

        SQLThread::SQLTasks task;
    };

    //常使用的插入指令队列
    struct SQLDataFrequentUsed
    {
        /*!
         * 构造插入数据指令
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

    //外部调用，连接到数据库
    void asyncConnectDb(const QString&, const QString&, const QString&);
    //外部调用，创建数据表
    void asyncCreatTable(const QString &);
    //外部调用,向表中插入数据
    void asyncInsertData(const QString&, const QString&, long, float);
    //外部调用，查询表
    void asyncSelectTable(const QString&);

private:
    SQL sql;
    QSemaphore freeSemaphore;
    QSemaphore usedSemaphore;
    QList<SQLDataRarelyUsed> rarelyUsed;
    QList<SQLDataFrequentUsed> frequentUsed;

    bool exitFlag;

Q_SIGNALS:
    //异步消息分发
     void openError(const QString&);
     //异步获取查询表的结果
     void getResult(MyVector&, int);
};
#endif