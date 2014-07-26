#include "modifystation.h"
#include "../include/settings.h"

#include <QDebug>

/*==================================================================    
 *    功能：   用于构造修改工作站界面， no为工作站编号，type指定
 *    输入参数：
 *    返回值： 
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
 *    功能：   初始化
 *    输入参数：no为工作站编号，type为工作站类型
 *    返回值： 
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

	//当点击确定时，执行的槽函数为ok
	connect(ui.okButton, SIGNAL(clicked()), this, SLOT(ok()));

	//记录当前焦点所在采集站的编号
	index = no;

	//读取已有的配置或增加新配置
	readIni(no);
}

/*==================================================================    
 *    功能：   
 *    输入参数：如果type为STATION，则是读取已有的配置。如为STATION_ADD则增加新配置页
 *    返回值： 
/*==================================================================*/
void ModifyStation::readIni(int no)
{
	//获取当前工作站数目
	int sNum = Settings::readStationNum();

	//添加工作站的页面
	if(type == STATION_ADD) {
		++sNum;
		ui.lineEdit_stationName->setText(Settings::readName(sNum));
		return;
	}
	//临时设置当前待添加station的名字
	ui.lineEdit_stationName->setText(Settings::readName(no));
	//当修改或者查看其他station
	ui.lineEdit_note->setText(Settings::readNote(no));

	//更新checkbox传感器
	checkMask(no);
}

/*==================================================================    
 *    功能：   检测传感器标志掩码
 *    输入参数：no为采集站编号
 *    返回值： 
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
 *    功能：   增加新配置或者更新已有配置
 *    输入参数：
 *    返回值： 
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
		//更新已有配置
		Settings::updateMaskAndNote(index, mask, note);
		emit updateStationSignal(index);
	}else {
		//增加新的采集站配置
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
