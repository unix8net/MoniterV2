#include "modifystation.h"
#include "../include/settings.h"
ModifyStation::ModifyStation(int no, QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	 setModal(true);
	 setWindowFlags(Qt::Window | Qt::FramelessWindowHint
		 | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint
		 | Qt::WindowMaximizeButtonHint);
	 setAttribute(Qt::WA_DeleteOnClose);
	 this->setAutoFillBackground(true);
	 QPalette palette;
	 palette.setColor(QPalette::Background, QColor(35,202,255));
	 this->setPalette(palette);
	 QPainterPath path;
	 QRectF rect = QRectF(this->rect());
	 path.addRoundRect(rect,10,10);
	 QPolygon polygon= path.toFillPolygon().toPolygon();
	 QRegion region(polygon);//根据这些点构造这个区域
	 setMask(region);
	 connect(ui.okButton,SIGNAL(clicked()),this,SLOT(ok()));
	 readIni(no);
}
void ModifyStation::ok()
{

}
void ModifyStation::readIni(int no)
{
	if(no == -1)
		return;
	ui.lineEdit_note->setText(Settings::readNote(no));
	ui.lineEdit_stationName->setText(Settings::readName(no));
	unsigned int mask = Settings::readStationSensorsByIndex(no);
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
ModifyStation::~ModifyStation()
{

}
