#include "../include/taskthread.h"

SQLThread::~SQLThread()
{
	exitFlag = true;
	usedSemaphore.release();
	freeSemaphore.release();
	wait();
}
SQLThread::SQLThread() :
	freeSemaphore(10), usedSemaphore(0)
{
	//10 > 10 + 3
	exitFlag = false;
	rarelyUsed.reserve(3);
	frequentUsed.reserve(10);
}

void SQLThread::run()
{
	bool haveError = false;
	int tryCnt = 3;

	while(true) {
		while(rarelyUsed.empty() && frequentUsed.empty()) {
			usedSemaphore.acquire();
			if(exitFlag) {
				rarelyUsed.clear();
				frequentUsed.clear();
				return;
			}
		}

		if(!rarelyUsed.empty()) {
			//连接数据库
			if( rarelyUsed.front().task == ConnectDb) {
				sql.setDb("test", rarelyUsed.front().user,
					rarelyUsed.front().passwd);
				if(!sql.openDb()) {
					emit openError("Can not open  MYSQL");
					if(++tryCnt <= 3) {
						asyncConnectDb("test", rarelyUsed.front().user,
							rarelyUsed.front().passwd);
					}
					else
						haveError = true;
					break;
				}

				if(!sql.createDb(rarelyUsed.front().name)) {
					emit openError("Create database failure");
					break;
				}

				if(!sql.changeDb(rarelyUsed.front().name)) {
					emit openError("Change database failure");
					break;
				}

			}
			else if( rarelyUsed.front().task == CreatTable)
			{
				if(! sql.createTable(rarelyUsed.front().name)) {
					emit openError("Create table failure");
					break;
				}
			}
			else if( rarelyUsed.front().task == SelectData) {
#if 0
				emit getResult(sql.selectTable(rarelyUsed.front().name, cnt), cnt);
#endif
			}

			rarelyUsed.pop_front();
			freeSemaphore.release();
			continue;
		}
		//处理第二队列
		if(!frequentUsed.empty()) {
			sql.insertData(frequentUsed.front().table,frequentUsed.front().time,
				frequentUsed.front().id, frequentUsed.front().data);
			frequentUsed.pop_front();
			freeSemaphore.release();
		}
	}
}

//连接数据库
void SQLThread::asyncConnectDb(const QString &db_name,const QString &user, const QString &passwd)
{
	freeSemaphore.acquire();
	rarelyUsed.push_back(SQLDataRarelyUsed(db_name,
		user, passwd, SQLThread::ConnectDb));
	usedSemaphore.release();

}
//创建表
void SQLThread::asyncCreatTable(const QString &table_)
{
	freeSemaphore.acquire();
	rarelyUsed.push_back(SQLDataRarelyUsed(table_,
		SQLThread::CreatTable));
	usedSemaphore.release();

}
void SQLThread::asyncInsertData(const QString& table_,const QString& t, long id, float data_)
{
	freeSemaphore.acquire();
	frequentUsed.push_back(SQLDataFrequentUsed(
		table_, t, id, data_));
	usedSemaphore.release();

}
void SQLThread::asyncSelectTable(const QString &table_)
{
	freeSemaphore.acquire();
	rarelyUsed.push_back(SQLDataRarelyUsed(table_,
		SQLThread::SelectData));
	usedSemaphore.release();
}

