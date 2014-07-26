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
	static  int readStationNum(); //��ȡ����վ��Ŀ
	static  unsigned int  readMask(int no);//��ȡ����������
	static  int  readLevel(int no, int sensor);//��ȡԤ��ֵ
	static QString readNote(int no); //��ȡ��ע
	static QString readName(int no);//��ȡ����
	static void setDataSouce(int type);
	static int readDataSouce();
private:
	static QSettings *softConfig;

};
#endif