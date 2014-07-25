#include <QSettings>
#include "../include/settings.h"
#include "../include/global.h"
QSettings *Settings::softConfig = new QSettings("cfg.ini",QSettings::IniFormat);

/*==================================================================    
 *    功能    ：   用于初始化ini文件，增加num为工作站数目，增加了一个demo 工作站
 *    输入参数：
 *    返回值  ： 
 *    作者    ：   
 *    日期    ：
/*==================================================================*/
void  Settings::initConfigFile()
{
	softConfig->beginGroup("Station");
	softConfig->setValue("num", 1);
	softConfig->endGroup();

	softConfig->beginGroup("S1");
	softConfig->setValue("SM", 0x01);
	softConfig->setValue("SN", "This is a demo station");
	softConfig->endGroup();
}


/*==================================================================    
 *    功能    ：   增加工作站
 *    输入参数：传感器掩码    备注
 *    返回值    ： 
 *    作者    ：   
 *    日期    ：
/*==================================================================*/
void Settings::addStation(unsigned int sensorMask, const QString &note)
{
	softConfig->beginGroup("Station");
	int num = (softConfig->value("num").toInt());
	++num;
	softConfig->setValue("num", num);
	softConfig->endGroup();

	QString str = "S" + QString::number(num, 10);
	softConfig->beginGroup(str);
	softConfig->setValue("SM", sensorMask); // sensor mask
	if(note.trimmed().length() == 0)
		softConfig->setValue("SN", "-");  //sensor note
	else
		softConfig->setValue("SN", note);

	for(int i = 0; i < SENSOR_TYPES; ++i)
		softConfig->setValue("SL" + QString::number(i, 10), 100000); //sensor level
	softConfig->endGroup();
}
/*==================================================================    
 *    功能    ：   读取采集站的数目
 *    输入参数：
 *    返回值    ： 
 *    作者    ：   
 *    日期    ：
/*==================================================================*/
unsigned int  Settings::readStationSensorsByIndex(int no)
{
	QString str = "S" + QString::number(no, 10);
	softConfig->beginGroup(str);
	int value = softConfig->value("SM").toUInt();
	softConfig->endGroup();
	return value;

}
/*==================================================================    
 *    功能    ：   读取采集站no的传感器sensor的预警值
 *    输入参数：
 *    返回值    ： 
 *    作者    ：   
 *    日期    ：
/*==================================================================*/
int  Settings::readStationLevelByIndex(int no, int sensor)
{
	QString str = "S" + QString::number(no, 10);
	softConfig->beginGroup(str);
	int value = softConfig->value("SL" + QString::number(sensor, 10)).toInt();
	softConfig->endGroup();
	return value;
}
/*==================================================================   
 *    功能    ：   
 *    输入参数：采集站编号  传感器编号  传感器阈值
 *    返回值    ： 
 *    作者    ：   
 *    日期    ：
/*==================================================================*/
void Settings::updateLevel(int no, int sensor, int value)
{
	QString str = "S" + QString::number(no, 10);
	softConfig->beginGroup(str);
	softConfig->setValue("SL" + QString::number(sensor, 10), value); //sensor level
	softConfig->endGroup();
}

QString Settings::readNote(int no)
{
	QString str = "S" + QString::number(no, 10);
	softConfig->beginGroup(str);
	QString value = softConfig->value("SN").toString();
	softConfig->endGroup();
	return value;
}
QString Settings::readName(int no)
{
	return "S" + QString::number(no, 10);
}