#include "netconfig.h"
#include <QHostInfo>
#include <QNetworkInterface>
#include "./dataWidget.h"
#include "../include/settings.h"
#include "./gomessage.h"
NetConfig::NetConfig(DataWidget *dw, QWidget *parent)
	: dataWidget(dw), QWidget(parent)
{
	ui = new Ui::NetConfig();
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
	
	//本机IP地址与本机名
	QString localHostName = QHostInfo::localHostName();
	ui->localmac->setText(localHostName);
	QHostInfo info = QHostInfo::fromName(localHostName);
	foreach(QHostAddress address,info.addresses())
	{
		if(address.protocol() == QAbstractSocket::IPv4Protocol && address.toString() != "127.0.0.1")
		{
			ui->localip->setText(address.toString());
			break;
		}
	}
}
void NetConfig::ok()
{
	if(Settings::readDataSouce() != 1) {
		goMessage *mes = new goMessage(QString::fromWCharArray(L"请先设置数据源"), 3000, this);
		mes->show();
		return;
	}
	if(dataWidget->getSocketFlag()== QAbstractSocket::ConnectedState){
		goMessage *mes = new goMessage(QString::fromWCharArray(L"服务器无需重复连接"), 3000, this);
		mes->show();
		return;
	}
	dataWidget->connectToServer(ui->serverip->text(), (ui->serverport->text().toUInt()));
	this->close();
}
void NetConfig::closeEvent(QCloseEvent*)
{
	delete ui;
	delete this;
}

NetConfig::~NetConfig()
{

}
