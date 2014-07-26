#include "uartconfig.h"
#include <QtExtSerialPort/qextserialport.h>
#include <QtExtSerialPort/qextserialenumerator.h>
#include "./gomessage.h"
#include "../include/settings.h"
UartConfig::UartConfig(QextSerialPort *tport, QWidget *parent)
	: port(tport), QWidget(parent)
{
	ui = new Ui::UartConfig();
	ui->setupUi(this);
	setWindowFlags(Qt::Window | Qt::FramelessWindowHint
		| Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint
		| Qt::WindowMaximizeButtonHint);
	//获取可用串口
	foreach (QextPortInfo info, QextSerialEnumerator::getPorts())
		ui->portBox->addItem(info.portName);

	ui->portBox->setEditable(true);
	ui->baudRateBox->addItem("1200", BAUD1200);
	ui->baudRateBox->addItem("2400", BAUD2400);
	ui->baudRateBox->addItem("4800", BAUD4800);
	ui->baudRateBox->addItem("9600", BAUD9600);
	ui->baudRateBox->addItem("19200", BAUD19200);
	ui->baudRateBox->addItem("38400", BAUD38400);
	ui->baudRateBox->addItem("115200", BAUD115200);
	ui->baudRateBox->setCurrentIndex(6);

	ui->parityBox->addItem("NONE", PAR_NONE);
	ui->parityBox->addItem("ODD", PAR_ODD);
	ui->parityBox->addItem("EVEN", PAR_EVEN);

	ui->dataBitsBox->addItem("5", DATA_5);
	ui->dataBitsBox->addItem("6", DATA_6);
	ui->dataBitsBox->addItem("7", DATA_7);
	ui->dataBitsBox->addItem("8", DATA_8);
	ui->dataBitsBox->setCurrentIndex(3);

	ui->stopBitsBox->addItem("1", STOP_1);
	ui->stopBitsBox->addItem("2", STOP_2);

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

	enumerator = new QextSerialEnumerator(this);
	enumerator->setUpNotifications();

	connect(ui->baudRateBox, SIGNAL(currentIndexChanged(int)), SLOT(onBaudRateChanged(int)));
	connect(ui->parityBox, SIGNAL(currentIndexChanged(int)), SLOT(onParityChanged(int)));
	connect(ui->dataBitsBox, SIGNAL(currentIndexChanged(int)), SLOT(onDataBitsChanged(int)));
	connect(ui->stopBitsBox, SIGNAL(currentIndexChanged(int)), SLOT(onStopBitsChanged(int)));

	connect(ui->pushButtonOk, SIGNAL(clicked()), SLOT(ok()));

	connect(ui->portBox, SIGNAL(editTextChanged(QString)), SLOT(onPortNameChanged(QString)));
	connect(enumerator, SIGNAL(deviceDiscovered(QextPortInfo)), SLOT(onPortAddedOrRemoved()));
	connect(enumerator, SIGNAL(deviceRemoved(QextPortInfo)), SLOT(onPortAddedOrRemoved()));
}


void UartConfig::closeEvent(QCloseEvent*)
{
	delete enumerator;
	delete ui;
	delete this;
}


void UartConfig::onPortNameChanged(const QString &)
{
	if (port->isOpen()){
		port->close();
	}
}


void UartConfig::onBaudRateChanged(int idx)
{
	port->setBaudRate((BaudRateType)ui->baudRateBox->itemData(idx).toInt());
}

void UartConfig::onParityChanged(int idx)
{
	port->setParity((ParityType)ui->parityBox->itemData(idx).toInt());
}

void UartConfig::onDataBitsChanged(int idx)
{
	port->setDataBits((DataBitsType)ui->dataBitsBox->itemData(idx).toInt());
}

void UartConfig::onStopBitsChanged(int idx)
{
	port->setStopBits((StopBitsType)ui->stopBitsBox->itemData(idx).toInt());
}
void UartConfig::onPortAddedOrRemoved()
{
	QString current = ui->portBox->currentText();
	ui->portBox->blockSignals(true);
	ui->portBox->clear();
	foreach (QextPortInfo info, QextSerialEnumerator::getPorts())
		ui->portBox->addItem(info.portName);
	ui->portBox->setCurrentIndex(ui->portBox->findText(current));
	ui->portBox->blockSignals(false);
}
void UartConfig::ok()
{
	if(Settings::readDataSouce() != 0) {
		goMessage *mes = new goMessage(QString::fromWCharArray(L"请先设置数据源"), 3000, this);
		mes->show();
		return;
	}
	if (!port->isOpen()) {
		port->setPortName(ui->portBox->currentText());
		if(port->open(QIODevice::ReadOnly)){
			this->close();
		}
		else{
			goMessage *mes = new goMessage(QString::fromWCharArray(L"串口已被占用"), 3000, this);
			mes->show();
		}

	}
	else {
		goMessage *mes = new goMessage(QString::fromWCharArray(L"串口已被占用"), 3000, this);
		mes->show();
		port->close();
	}
	
}

UartConfig::~UartConfig()
{
}
