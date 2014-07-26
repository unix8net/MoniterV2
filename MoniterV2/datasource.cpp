#include "datasource.h"
#include "./include/settings.h"
DataSource::DataSource(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setWindowFlags(Qt::Window | Qt::FramelessWindowHint
		| Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint
		| Qt::WindowMaximizeButtonHint);

	this->setAutoFillBackground(true);
	QPalette palette;
	palette.setColor(QPalette::Background, QColor(35,202,25));
	this->setPalette(palette);

	QPainterPath path;
	QRectF rect = QRectF(this->rect());
	path.addRoundRect(rect,10,10);
	QPolygon polygon= path.toFillPolygon().toPolygon();
	QRegion region(polygon);
	setMask(region);

	connect(ui.pushButton_ok, SIGNAL(clicked()),this,SLOT(ok()));
}

void DataSource::ok()
{
	if(ui.radioButtonUart->isChecked())
		Settings::setDataSouce(0);
	else if(ui.radioButtonNet->isChecked()){
		Settings::setDataSouce(1);
	}
	else
		Settings::setDataSouce(-1);
	this->close();
}
DataSource::~DataSource()
{
}
void DataSource::closeEvent(QCloseEvent *)
{
	delete this;
}