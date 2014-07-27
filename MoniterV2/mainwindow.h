#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include "ui_mainwindow.h"
#include "../include/view.h"

class QHBoxLayout;
class QDockWidget;
class TreeWidget;
class QTreeWidgetItem;
class TableWidget;
class DataWidget;
class HLed;
class HLeds;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	enum Leds
	{
		SOFT_SYSTEM_LED,
		MYSQL_LED,
		TCPSERVER_LED,
		UART_LED,
		N_LEDS

	};
	MainWindow(QWidget *parent = 0);
	~MainWindow();

	HLed* led[N_LEDS];
	HLeds* leds;

private:
	Ui::MainWindowClass ui;
	//QDockWidget *mainDock;
	TreeWidget *mainTreeWidget;
	TableWidget *mainTableWidget;
	DataWidget *dataWidget;
	void initTreeWidget();
	void initTableWidget();
	void initTableWidgetData();


private Q_SLOTS:
	void acceptFromTreeWidget(QTreeWidgetItem * item, int column);
	void addNewTableItem();
	void updateTableItem(int);
	void updateLeds();


};

#endif // MAINWINDOW_H
