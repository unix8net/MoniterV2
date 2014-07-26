#ifndef SQL_H
#define SQL_H

#include <QVector>
#include <QPointF>
#include <QtSql>
#include <QSqlDatabase>

/*==================================================================
 *    作者：Creater
 *    简介： 数据库操作接口
 *    主页： unix8.net
 *    日期：2014-04-27
/*==================================================================*/


/*!
 * 用于在查询表的回调槽函数时能够作为参数
 * */
typedef QVector<QPointF> MyVector;
Q_DECLARE_METATYPE(MyVector)

/*!
  * MYSQL操作类
*/
class SQL
{
private:
    QSqlDatabase db;
    //当前的表
    QString table;
    //当前数据库
    QString dbname;
    //用于select返回结果集
   // MyVector vec;
public:

    ~SQL(){db.close();}
    SQL(){ db = QSqlDatabase::addDatabase("QMYSQL");/*vec.reserve(1441);*/}
    void setDb(QString db_name, QString user, QString pw);
    bool openDb();
    bool changeDb(QString db_name);
    bool createDb(QString db_name);
    bool createTable(const QString &table_);
    bool insertData(const QString &table_, const QString &t, long id, float d);
#if 0
    MyVector& selectTable(QString table, int& s);
    MyVector&  getResult();
#endif
    SQL(QSqlDatabase& db_):db(db_){ /*vec.reserve(1441);*/}

};

#endif

