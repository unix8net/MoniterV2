#include <QSettings>
#include "../include/settings.h"
#include "../include/global.h"
QSettings *Settings::softConfig = new QSettings("cfg.ini",QSettings::IniFormat);

/*==================================================================    
 *    ����    ��   ���ڳ�ʼ��ini�ļ�������numΪ����վ��Ŀ��������һ��demo ����վ
 *    ���������
 *    ����ֵ  �� 
 *    ����    ��   
 *    ����    ��
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
 *    ����    ��   ���ӹ���վ
 *    �������������������    ��ע
 *    ����ֵ    �� 
 *    ����    ��   
 *    ����    ��
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
 *    ����    ��   ��ȡ�ɼ�վ����Ŀ
 *    ���������
 *    ����ֵ    �� 
 *    ����    ��   
 *    ����    ��
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
 *    ����    ��   ��ȡ�ɼ�վno�Ĵ�����sensor��Ԥ��ֵ
 *    ���������
 *    ����ֵ    �� 
 *    ����    ��   
 *    ����    ��
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
 *    ����    ��   
 *    ����������ɼ�վ���  ���������  ��������ֵ
 *    ����ֵ    �� 
 *    ����    ��   
 *    ����    ��
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