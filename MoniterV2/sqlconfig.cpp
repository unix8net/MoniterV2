#include "sqlconfig.h"
#include "../include/settings.h"
#include <QSettings>
#include "./dataWidget.h"
SqlConfig::SqlConfig(DataWidget *dw, QWidget *parent)
	: dataWidget(dw), QWidget(parent)
{
	ui = new Ui::SqlConfig();
	ui->setupUi(this);

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

	connect(ui->pushButtonOk, SIGNAL(clicked()), SLOT(ok()));
}
void SqlConfig::ok()
{
	Settings::softConfig->beginGroup("MySQL");
	Settings::softConfig->setValue("dbname", ui->dbname->text());
	Settings::softConfig->setValue("user", ui->user->text());
	Settings::softConfig->setValue("hostname", ui->hostname->text());
	Settings::softConfig->setValue("passwd", ui->password->text());
	Settings::softConfig->endGroup();
	dataWidget->getSqlThread().asyncConnectDb(ui->dbname->text(), ui->user->text(), ui->password->text());
	this->close();
}
void SqlConfig::closeEvent(QCloseEvent*)
{
	delete ui;
	delete this;
}

SqlConfig::~SqlConfig()
{

}
