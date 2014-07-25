#include "mainwindow.h"
#include <QtWidgets/QApplication>
#include <QtCore/QCoreApplication>
//#include <QSplashScreen>
#include "../include/settings.h"
int main(int argc, char *argv[])
{
	QCoreApplication::setOrganizationName("Moniter");
	QCoreApplication::setOrganizationDomain("unix8.net");
	QCoreApplication::setApplicationName("MoniterV2");
	QApplication a(argc, argv);
	//QPixmap pixmap("./img/splashscreen.png");
	//QSplashScreen *splash = new QSplashScreen(pixmap);
	//splash->show();
	MainWindow w;
	w.show();
	//splash->finish(&w);
	//delete splash;
	/*
	QSettings settings("cfg.ini",QSettings::IniFormat);
	settings.beginGroup("Station");
	settings.setValue("num", 100);
	settings.setValue("sensor", 100);
	settings.setValue("fullScreen", "test");
	settings.endGroup();

	*/
	Settings::initConfigFile();
	//Settings::addStation(1);
	return a.exec();
}
