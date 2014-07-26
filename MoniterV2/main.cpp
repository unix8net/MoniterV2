#include "mainwindow.h"
#include <QtWidgets/QApplication>
#include <QtCore/QCoreApplication>
//#include <QSplashScreen>
#include <QtExtSerialPort/qextserialport.h>
#include "../include/settings.h"

#include "../include/plot.h"
#include "dataWidget.h"
#include "../include/timebus.h"
#include "../include/taskthread.h"
#include "../include/infowidget.h"
int main(int argc, char *argv[])
{
	//QCoreApplication::setOrganizationName("Moniter");
	//QCoreApplication::setOrganizationDomain("unix8.net");
	//QCoreApplication::setApplicationName("MoniterV2");
	QApplication a(argc, argv);

	//QPixmap pixmap("./img/splashscreen.png");
	//QSplashScreen *splash = new QSplashScreen(pixmap);
	//splash->show();

MainWindow w;
	w.show();

	//splash->finish(&w);
	//delete splash;

	//Settings::initConfigFile();
	/*
	Plot *plot = new Plot();
	plot->show();

	
	TimeBus timeBus;
	SQLThread sqlthread;
	QextSerialPort* serialPort;
	{
		//´®¿ÚÅäÖÃ
		PortSettings settings = {BAUD9600, DATA_8, PAR_NONE, STOP_1, FLOW_OFF, 10};
		//´®¿Ú
		serialPort = new QextSerialPort("COM1", settings);
	}
	DataWidget dw(sqlthread,timeBus, serialPort);
	dw.show();*/
	return a.exec();
}
