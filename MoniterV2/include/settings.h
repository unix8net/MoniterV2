#ifndef SETTINGS_H
#define SETTINGS_H


class QSettings;
class Settings
{
public:
	static	void initConfigFile();
	static  void addStation(unsigned int sensorMask, const QString &note = "");
	static  void updateLevel(int no, int sensor, int value);
	static  int readStationNum();
	static  unsigned int  readStationSensorsByIndex(int no);//读取传感器
	static  int  readStationLevelByIndex(int no, int sensor);//读取预警值
	static QString readNote(int no);
	static QString readName(int no);

private:
	static QSettings *softConfig;

};
#endif