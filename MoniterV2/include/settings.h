#ifndef SETTINGS_H
#define SETTINGS_H


class QSettings;
class Settings
{
public:
	static	void initConfigFile();
	static  void addStation(unsigned int mask, const QString &note = "");
	static  void updateLevel(int no, int sensor, int value);
	static  void updateMaskAndNote(int no, unsigned int mask, QString&);
	static  int readStationNum(); //读取工作站数目
	static  unsigned int  readMask(int no);//读取传感器掩码
	static  int  readLevel(int no, int sensor);//读取预警值
	static QString readNote(int no); //读取备注
	static QString readName(int no);//读取名字
	static void setDataSouce(int type);
	static int readDataSouce();
private:
	static QSettings *softConfig;

};
#endif