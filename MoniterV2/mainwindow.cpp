#include "mainwindow.h"
#include <QHBoxLayout>

#include <QFrame>
#include <QDockWidget>
#include <QTextEdit>
#include <QIcon>
//#include <QTableWidget>
#include "modifystation.h"
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

	//QFrame *mainFrame = new QFrame(mainWidget);
	mainTableWidget = new TableWidget(mainWidget);
	initTableWidget();
	hBoxLayout = new QHBoxLayout(mainWidget);
	hBoxLayout->addWidget(mainTableWidget);
	//hBoxLayout->addWidget(mainFrame);
	//QTextEdit *te1=new QTextEdit(mainFrame);  
	//te1->setText(tr("Window1,The dock widget can be moved between docks by the user"));  
	//hBoxLayout->addWidget(te1);
	mainWidget->setLayout(hBoxLayout);

}

void MainWindow::acceptFromTreeWidget(QTreeWidgetItem * item, int column)
{
	switch (item->type()) {
	case TreeWidget::STATION_ADD:
		ModifyStation *modifyStation = new ModifyStation(this);
		modifyStation->show();
		break;
	}
}
void MainWindow::initTableWidget()
{
	mainTableWidget->setColumnCount(9); 
	mainTableWidget->setRowCount(5); 
	
	//mainTableWidget->resizeColumnsToContents();
	mainTableWidget->horizontalHeader()->setStretchLastSection(true);  
	//mainTableWidget->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);
	mainTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); 
	mainTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);  
	mainTableWidget->verticalHeader()->setVisible(false); 
	QStringList headers; 
	headers << QString::fromWCharArray(L"采集站名") <<QString::fromWCharArray(L"传感器数") << 
		QString::fromWCharArray(L"雨量值") << QString::fromWCharArray(L"应力值") <<QString::fromWCharArray(L"震动值")
		<<QString::fromWCharArray(L"倾角值")<<QString::fromWCharArray(L"温湿度值")
		<<QString::fromWCharArray(L"状态")<<QString::fromWCharArray(L"备注"); 
	mainTableWidget->setHorizontalHeaderLabels(headers); 
	mainTableWidget->setItem(0, 0, new QTableWidgetItem(QString("采集站 #1"))); 
	mainTableWidget->setItem(1, 0, new QTableWidgetItem(QString("采集站 #1"))); 
	mainTableWidget->setItem(2, 0, new QTableWidgetItem(QString("3"))); 
	mainTableWidget->setItem(3, 0, new QTableWidgetItem(QString("4"))); 
	mainTableWidget->setItem(4, 0, new QTableWidgetItem(QString("5"))); 
	mainTableWidget->setItem(0, 1, new QTableWidgetItem(tr("20100112"))); 
}
void MainWindow::initTreeWidget()
{
	mainDock=new QDockWidget(QString::fromWCharArray(L"监测与预警系统选项"),this);
	mainDock->setFeatures(QDockWidget::DockWidgetMovable);  
	mainDock->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);  
	mainTreeWidget = new TreeWidget(mainDock);

	QTreeWidgetItem *itemCollectionStation = new QTreeWidgetItem(QStringList(QString::fromWCharArray(L"采集站")));
	itemCollectionStation->setIcon(0, QIcon("./img/01.png"));
	mainTreeWidget->addTreeLeaf(itemCollectionStation,QString::fromWCharArray(L"采集站 #1"), TreeWidget::STATION);
	mainTreeWidget->addTreeLeaf(itemCollectionStation,QString::fromWCharArray(L"采集站 #2"), TreeWidget::STATION);
	mainTreeWidget->addTreeLeaf(itemCollectionStation,QString::fromWCharArray(L"采集站 #3"), TreeWidget::STATION);
	mainTreeWidget->addTreeLeaf(itemCollectionStation,QString::fromWCharArray(L"增加采集站"), TreeWidget::STATION_ADD);
	mainTreeWidget->addTopLevelItem(itemCollectionStation);
	itemCollectionStation->setExpanded(true);

	


	QTreeWidgetItem *itemDataDisplay = new QTreeWidgetItem(QStringList(QString::fromWCharArray(L"数据显示")));
	itemDataDisplay->setIcon(0, QIcon("./img/02.png"));
	mainTreeWidget->addTreeLeaf(itemDataDisplay,QString::fromWCharArray(L"数据动态显示"), TreeWidget::DATA_DYNAMIC_DISPLAY);
	mainTreeWidget->addTreeLeaf(itemDataDisplay,QString::fromWCharArray(L"数据实时波形"), TreeWidget::DATA_REALTIME_WAVE);
	mainTreeWidget->addTreeLeaf(itemDataDisplay,QString::fromWCharArray(L"历史数据波形"), TreeWidget::DATA_HISTORY_WAVE);
	mainTreeWidget->addTreeLeaf(itemDataDisplay,QString::fromWCharArray(L"历史数据查询"), TreeWidget::DATA_QUERY);
	mainTreeWidget->addTopLevelItem(itemDataDisplay);
	itemDataDisplay->setExpanded(true);



	QTreeWidgetItem *itemConfig = new QTreeWidgetItem(QStringList(QString::fromWCharArray(L"系统配置")));
	itemConfig->setIcon(0, QIcon("./img/03.png"));
	mainTreeWidget->addTreeLeaf(itemConfig,QString::fromWCharArray(L"数据来源设置"), TreeWidget::DATA_SOURCE);
	mainTreeWidget->addTreeLeaf(itemConfig,QString::fromWCharArray(L"串口设置"), TreeWidget::CFG_UART);
	mainTreeWidget->addTreeLeaf(itemConfig,QString::fromWCharArray(L"网络设置"), TreeWidget::CFG_NET);
	mainTreeWidget->addTreeLeaf(itemConfig,QString::fromWCharArray(L"数据库设置"), TreeWidget::CFG_SQL);
	mainTreeWidget->addTreeLeaf(itemConfig,QString::fromWCharArray(L"配置文件设置"), TreeWidget::CFG_FILE);
	mainTreeWidget->addTreeLeaf(itemConfig,QString::fromWCharArray(L"阈值设置"), TreeWidget::CFG_LEVEL);
	mainTreeWidget->addTopLevelItem(itemConfig);

	QTreeWidgetItem *itemAlgorithm = new QTreeWidgetItem(QStringList(QString::fromWCharArray(L"数据处理")));
	itemAlgorithm->setIcon(0, QIcon("./img/04.png"));
	mainTreeWidget->addTreeLeaf(itemAlgorithm,QString::fromWCharArray(L"数据处理模型"), TreeWidget::ALGORITHM1);
	mainTreeWidget->addTreeLeaf(itemAlgorithm,QString::fromWCharArray(L"数据处理算法"), TreeWidget::ALGORITHM1);
	mainTreeWidget->addTopLevelItem(itemAlgorithm);

	QTreeWidgetItem *help = new QTreeWidgetItem(QStringList(QString::fromWCharArray(L"关于与帮助")));
	help->setIcon(0, QIcon("./img/help.png"));
	mainTreeWidget->addTreeLeaf(help,QString::fromWCharArray(L"关于"), TreeWidget::ABOUT);
	mainTreeWidget->addTreeLeaf(help,QString::fromWCharArray(L"帮助"), TreeWidget::HELP);
	mainTreeWidget->addTopLevelItem(help);

	mainDock->setWidget(mainTreeWidget);  
	addDockWidget(Qt::LeftDockWidgetArea,mainDock); 

	 connect(mainTreeWidget,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),this,SLOT(acceptFromTreeWidget(QTreeWidgetItem*,int)));
}
MainWindow::~MainWindow()
{

}
