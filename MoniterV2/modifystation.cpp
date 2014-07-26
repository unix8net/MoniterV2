#include "modifystation.h"
#include "../include/settings.h"

#include <QDebug>

/*==================================================================    
 *    ���ܣ�   ���ڹ����޸Ĺ���վ���棬 noΪ����վ��ţ�typeָ��
 *    ���������
 *    ����ֵ�� 
/*==================================================================*/
ModifyStation::ModifyStation(int no, QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	type = STATION;
	initModifyStation(no);
}

ModifyStation::ModifyStation(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	type = STATION_ADD;
	initModifyStation(-1);
}

/*==================================================================    
 *    ���ܣ�   ��ʼ��
 *    ���������noΪ����վ��ţ�typeΪ����վ����
 *    ����ֵ�� 
/*==================================================================*/
void ModifyStation::initModifyStation(int no)
{
	setModal(true);
	setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowSystemMenuHint 
		| Qt::WindowMinimizeButtonHint
		| Qt::WindowMaximizeButtonHint);

	//setAttribute(Qt::WA_DeleteOnClose);

	this->setAutoFillBackground(true);
	QPalette palette;
	palette.setColor(QPalette::Background, QColor(35,202,255));
	this->setPalette(palette);

	QPainterPath path;
	QRectF rect = QRectF(this->rect());
	path.addRoundRect(rect,10,10);
	QPolygon polygon= path.toFillPolygon().toPolygon();
	QRegion region(polygon);
	setMask(region);

	//�����ȷ��ʱ��ִ�еĲۺ���Ϊok
	connect(ui.okButton, SIGNAL(clicked()), this, SLOT(ok()));

	//��¼��ǰ�������ڲɼ�վ�ı��
	index = no;

	//��ȡ���е����û�����������
	readIni(no);
}

/*==================================================================    
 *    ���ܣ�   
 *    ������������typeΪSTATION�����Ƕ�ȡ���е����á���ΪSTATION_ADD������������ҳ
 *    ����ֵ�� 
/*==================================================================*/
void ModifyStation::readIni(int no)
{
	//��ȡ��ǰ����վ��Ŀ
	int sNum = Settings::readStationNum();

	//��ӹ���վ��ҳ��
	if(type == STATION_ADD) {
		++sNum;
		ui.lineEdit_stationName->setText(Settings::readName(sNum));
		return;
	}
	//��ʱ���õ�ǰ�����station������
	ui.lineEdit_stationName->setText(Settings::readName(no));
	//���޸Ļ��߲鿴����station
	ui.lineEdit_note->setText(Settings::readNote(no));

	//����checkbox������
	checkMask(no);
}

/*==================================================================    
 *    ���ܣ�   ��⴫������־����
 *    ���������noΪ�ɼ�վ���
 *    ����ֵ�� 
/*==================================================================*/
void ModifyStation::checkMask(int no)
{
	unsigned int mask = Settings::readMask(no);
	if(mask & 0x01)
		ui.checkBox_angle->setChecked(true);
	else
		ui.checkBox_angle->setChecked(false);
	if(mask & 0x02)
		ui.checkBox_inDis->setChecked(true);
	else
		ui.checkBox_inDis->setChecked(false);
	if(mask & 0x04)
		ui.checkBox_outDis->setChecked(true);
	else
		ui.checkBox_outDis->setChecked(false);
	if(mask & 0x08)
		ui.checkBox_rain->setChecked(true);
	else
		ui.checkBox_rain->setChecked(false);
	if(mask & 0x10)
		ui.checkBox_stress->setChecked(true);
	else
		ui.checkBox_stress->setChecked(false);
}

/*==================================================================    
 *    ���ܣ�   ���������û��߸�����������
 *    ���������
 *    ����ֵ�� 
/*==================================================================*/
void ModifyStation::ok()
{
	unsigned int mask = 0x00;
	if(ui.checkBox_angle->isChecked()) {
		mask |= 0x01;
	}
	if(ui.checkBox_inDis->isChecked()) {
		mask |= 0x02;
	}
	if(ui.checkBox_outDis->isChecked()) {
		mask |= 0x04;
	}
	if(ui.checkBox_rain->isChecked()) {
		mask |= 0x08;
	}
	if(ui.checkBox_stress->isChecked()) {
		mask |= 0x10;
	}

	QString note = ui.lineEdit_note->text();

	if(type == STATION){
		//������������
		Settings::updateMaskAndNote(index, mask, note);
		emit updateStationSignal(index);
	}else {
		//�����µĲɼ�վ����
		Settings::addStation(mask, note);
		emit addNewStationSignal();
	}
	this->close();
}

void ModifyStation::closeEvent(QCloseEvent *)
{
	delete this;
}

ModifyStation::~ModifyStation()
{
}
