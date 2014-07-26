#include "../include/sql.h"
/*!
 * \brief SQL::setDb 配置数据库信息
 * \param db_name 数据库名
 * \param user 用户
 * \param pw 密码
 */
void SQL::setDb(QString db_name, QString user, QString pw)
{
    dbname = db_name;
    db.setDatabaseName(db_name);
    db.setHostName("localhost");
    db.setUserName(user);
    db.setPassword(pw);
}
/*!
 * \brief SQL::openDb 打开数据库连接
 * \return 连接状态
 */
bool SQL::openDb()
{
    return db.open();
}
/*!
 * \brief SQL::changeDb 更改数据库
 * \param db_name
 * \return
 */
bool SQL::changeDb(QString db_name)
{
    QSqlQuery query;
    dbname = db_name;
    QString s = "use  "+ db_name;
    return query.exec(s);
}
/*!
 * \brief SQL::createDb 创建一个数据库,如果该数据库已经存在，则忽略
 * \param db_name
 * \return
 */
bool SQL::createDb(QString db_name)
{
     QSqlQuery query;
    dbname = db_name;
    QString s = "Create Database If Not Exists  " + db_name;
    return query.exec(s);
}
/*!
 * \brief SQL::createTable  创建表，列为 id + 时间time + 数据data
 * \param table_
 * \return
 */
bool SQL::createTable(const QString &table_)
{
    QSqlQuery query;
    table = table_;
    QString s = "CREATE TABLE   If Not Exists "+ table+ "(id INTEGER PRIMARY KEY, time timestamp, data float )";
    return query.exec(s);
}
/*!
 * \brief SQL::insertData 向表中插入数据
 * \param table_
 * \param intime
 * \param t
 * \param d
 * \return
 */
bool SQL::insertData(const QString &table_,const QString &intime, long t, float d)
{
    QSqlQuery query;
    QString s = "INSERT INTO " + table_+"(id, time, data) VALUES (:id, :time, :data)";
    query.prepare(s);
    query.bindValue(":id", t);
    query.bindValue(":time", intime);
    query.bindValue(":data", d);
    return query.exec();
}
/*!
 * \brief SQL::selectTable 查询数据，并以vector返回
 * \param table
 * \param cnt
 * \return
 */
#if 0
MyVector& SQL::selectTable(QString table, int& cnt)
{
    QSqlQuery query;
    QString s = "SELECT id, time, data FROM " + table;
    query.exec(s);
    vec.clear();
    while (query.next())
    {
        vec.push_back(QPointF(query.value(0).toInt(),query.value(2).toFloat()));
    }
    cnt = vec.size();
    return vec;
}
MyVector& SQL::getResult()
{
    return vec;
}
#endif
