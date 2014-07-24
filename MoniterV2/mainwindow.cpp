#include "mainwindow.h"
#include <QHBoxLayout>
#include "../include/view.h"
#include <QFrame>
#include <QDockWidget>
#include <QTextEdit>
#include <QIcon>
MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	QWidget* mainWidget = new QWidget(this);
	this->setCentralWidget(mainWidget);
	this->resize(800, 600);
	/*
	setWindowFlags(Qt::Window | Qt::FramelessWindowHint
		| Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint
		| Qt::WindowMaximizeButtonHint);
		*/
	
	initTreeWidget();

	QFrame *mainFrame = new QFrame(mainWidget);
	hBoxLayout = new QHBoxLayout(mainWidget);
	hBoxLayout->addWidget(mainFrame);
	QTextEdit *te1=new QTextEdit(mainFrame);  
	te1->setText(tr("Window1,The dock widget can be moved between docks by the user"));  
	hBoxLayout->addWidget(dock);
	hBoxLayout->addWidget(te1);
	mainWidget->setLayout(hBoxLayout);

}
void MainWindow::initTableWidget()
{
	
}
void MainWindow::initTreeWidget()
{
	mainDock=new QDockWidget(QString::fromWCharArray(L"监测与预警系统选项"),this);
	mainDock->setFeatures(QDockWidget::DockWidgetMovable);  
	mainDock->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);  
	mainTreeWidget = new TreeWidget(mainDock);

	QTreeWidgetItem *itemCollectionStation = new QTreeWidgetItem(QStringList(QString::fromWCharArray(L"采集站")),ItemType1);
	itemCollectionStation->setIcon(0, QIcon("./img/01.png"));
	mainTreeWidget->addTreeLeaf(itemCollectionStation,QString::fromWCharArray(L"采集站 #1"), ItemType2);
	mainTreeWidget->addTreeLeaf(itemCollectionStation,QString::fromWCharArray(L"采集站 #2"), ItemType2);
	mainTreeWidget->addTreeLeaf(itemCollectionStation,QString::fromWCharArray(L"采集站 #3"), ItemType2);
	mainTreeWidget->addTreeLeaf(itemCollectionStation,QString::fromWCharArray(L"增加采集站"), ItemType3);
	mainTreeWidget->addTopLevelItem(itemCollectionStation);
	itemCollectionStation->setExpanded(true);

	


	QTreeWidgetItem *itemDataDisplay = new QTreeWidgetItem(QStringList(QString::fromWCharArray(L"数据显示")),ItemType1);
	itemDataDisplay->setIcon(0, QIcon("./img/02.png"));
	mainTreeWidget->addTreeLeaf(itemDataDisplay,QString::fromWCharArray(L"数据动态显示"), ItemType2);
	mainTreeWidget->addTreeLeaf(itemDataDisplay,QString::fromWCharArray(L"数据实时波形"), ItemType2);
	mainTreeWidget->addTreeLeaf(itemDataDisplay,QString::fromWCharArray(L"历史数据查询"), ItemType2);
	mainTreeWidget->addTreeLeaf(itemDataDisplay,QString::fromWCharArray(L"历史数据查询"), ItemType2);
	mainTreeWidget->addTopLevelItem(itemDataDisplay);
	itemDataDisplay->setExpanded(true);



	QTreeWidgetItem *itemConfig = new QTreeWidgetItem(QStringList(QString::fromWCharArray(L"系统配置")),ItemType1);
	itemConfig->setIcon(0, QIcon("./img/03.png"));
	mainTreeWidget->addTreeLeaf(itemConfig,QString::fromWCharArray(L"数据来源设置"), ItemType2);
	mainTreeWidget->addTreeLeaf(itemConfig,QString::fromWCharArray(L"串口设置"), ItemType2);
	mainTreeWidget->addTreeLeaf(itemConfig,QString::fromWCharArray(L"网络设置"), ItemType2);
	mainTreeWidget->addTreeLeaf(itemConfig,QString::fromWCharArray(L"数据库设置"), ItemType2);
	mainTreeWidget->addTreeLeaf(itemConfig,QString::fromWCharArray(L"配置文件设置"), ItemType2);
	mainTreeWidget->addTreeLeaf(itemConfig,QString::fromWCharArray(L"阈值设置"), ItemType2);
	mainTreeWidget->addTopLevelItem(itemConfig);

	QTreeWidgetItem *itemAlgorithm = new QTreeWidgetItem(QStringList(QString::fromWCharArray(L"数据处理")),ItemType1);
	itemAlgorithm->setIcon(0, QIcon("./img/04.png"));
	mainTreeWidget->addTreeLeaf(itemAlgorithm,QString::fromWCharArray(L"数据处理模型"), ItemType2);
	mainTreeWidget->addTreeLeaf(itemAlgorithm,QString::fromWCharArray(L"数据处理算法"), ItemType2);
	mainTreeWidget->addTopLevelItem(itemAlgorithm);

	QTreeWidgetItem *help = new QTreeWidgetItem(QStringList(QString::fromWCharArray(L"关于与帮助")),ItemType1);
	help->setIcon(0, QIcon("./img/help.png"));
	mainTreeWidget->addTreeLeaf(help,QString::fromWCharArray(L"关于"), ItemType2);
	mainTreeWidget->addTreeLeaf(help,QString::fromWCharArray(L"帮助"), ItemType2);
	mainTreeWidget->addTopLevelItem(help);

	mainDock->setWidget(mainTreeWidget);  
	addDockWidget(Qt::LeftDockWidgetArea,mainDock);  
}
MainWindow::~MainWindow()
{

}
