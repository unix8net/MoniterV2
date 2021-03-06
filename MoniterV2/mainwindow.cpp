#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QBrush>

#include "modifystation.h"
#include "mainwindow.h"
#include "../include/settings.h"
#include "../include/global.h"
#include "dataWidget.h"
#include "./datasource.h"
#include "./uartconfig.h"
#include "./netconfig.h"
#include "./sqlconfig.h"
#include "./include/hled.h"
#include "./include/hleds.h"
MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	QWidget* mainWidget = new QWidget(this);
	this->setCentralWidget(mainWidget);
	setWindowState(Qt::WindowMaximized);	

	//background
	this->setAutoFillBackground(true);
	QPalette palette;
	palette.setColor(QPalette::Background, QColor(35,202,255));
	//palette.setColor(QPalette::Background, Qt::white);
	this->setPalette(palette);

	mainTableWidget = new TableWidget(mainWidget);
	mainTreeWidget = new TreeWidget(mainWidget);
	initTableWidget();
	initTreeWidget();

	dataWidget = new DataWidget(this);
	QHBoxLayout *hBoxLayout = new QHBoxLayout(mainWidget);
	QVBoxLayout *vHboxLayoout = new QVBoxLayout(mainWidget);
	vHboxLayoout->addWidget(mainTableWidget,1);
	vHboxLayoout->addWidget(dataWidget,3);
	hBoxLayout->addLayout(vHboxLayoout, 8);
	hBoxLayout->addWidget(mainTreeWidget, 1);
	mainWidget->setLayout(hBoxLayout);
	statusBar()->setStyleSheet(QString("QStatusBar::item{border:0px}"));
	for(int i = 0 ; i < N_LEDS; ++i)
		led[i] = new HLed(this);
	leds = new HLeds(this);
	led[SOFT_SYSTEM_LED]->setHaveText(true);
	led[SOFT_SYSTEM_LED]->setText(QString::fromWCharArray(L"软件系统"));
	led[SOFT_SYSTEM_LED]->turnOn();
	statusBar()->addWidget(led[SOFT_SYSTEM_LED], 1);


	led[MYSQL_LED]->setHaveText(true);
	led[MYSQL_LED]->setText(QString::fromWCharArray(L"MySQL数据库"));
	led[MYSQL_LED]->turnOn();
	statusBar()->addWidget(led[MYSQL_LED], 1);


	led[TCPSERVER_LED]->setHaveText(true);
	led[TCPSERVER_LED]->setText(QString::fromWCharArray(L"TCP服务器连接"));
	led[TCPSERVER_LED]->turnOff();
	statusBar()->addWidget(led[TCPSERVER_LED], 1);



	led[UART_LED]->setHaveText(true);
	led[UART_LED]->setText(QString::fromWCharArray(L"串口使用"));
	led[UART_LED]->turnOff();
	statusBar()->addWidget(led[UART_LED], 3);

	statusBar()->addWidget(leds, 1);
}

/*==================================================================    
 *    功能：   该函数用于接收TreeWidget的Item双击信号
 *    输入参数：
 *    返回值： 
/*==================================================================*/
void MainWindow::acceptFromTreeWidget(QTreeWidgetItem * item, int column)
{
	switch (item->type()) {

		case TreeWidget::STATION:{
			
			//获取station的编号，因为不同的Station有不同的编号
			QString id = item->text(0);
			int index = id.remove(0, id.lastIndexOf("#") + 1).toInt();
			
			ModifyStation *modifyStation = new ModifyStation(index, this);//-1
			modifyStation->changleTitle(QString::fromWCharArray(L"更改已知采集站信息"));

			//采集站修改后点击OK
			connect(modifyStation, SIGNAL(updateStationSignal(int)), this, SLOT(updateTableItem(int)));
			modifyStation->show();
			break;
		}

		case TreeWidget::STATION_ADD: {
			ModifyStation *modifyStation = new ModifyStation(this);
			modifyStation->changleTitle(QString::fromWCharArray(L"增加新采集站信息"));

			//采集站增加信息填好后点击OK
			connect(modifyStation, SIGNAL(addNewStationSignal()), this, SLOT(addNewTableItem()));
			modifyStation->show();
			break;
			}

		case TreeWidget::DATA_SOURCE: {
			DataSource *ds = new DataSource();
			ds->show();
			break;
			}

		case TreeWidget::CFG_UART: {
			UartConfig *uart = new UartConfig(dataWidget->getSerialPort(), this);
			uart->show();
			break;
		}
		case TreeWidget::CFG_NET: {
			NetConfig *net = new NetConfig(dataWidget);
			net->show();
			break;
		}
		case TreeWidget::CFG_SQL: {
			SqlConfig *sql = new SqlConfig(dataWidget);
			sql->show();
			break;
								  }
	}
}

/*==================================================================    
 *    功能：   在tableWidget中增加新的行
 *    输入参数：
 *    返回值： 
/*==================================================================*/
void MainWindow::addNewTableItem()
{
	initTableWidgetData();
}

/*==================================================================    
 *    功能：   更新tableWidget中的某行
 *    输入参数：index为采集站编号，从1开始编号
 *    返回值： 
/*==================================================================*/
void MainWindow::updateTableItem(int index)
{
	unsigned int mask = Settings::readMask(index);
	QTableWidgetItem *item;
	int sensors = 0;

	for(int j = 2;  j  <= SENSOR_TYPES + 1; ++j) {
		item = mainTableWidget->item(index - 1, j);
		if(mask & 0x01) {
			++sensors;
			item->setBackground(QBrush(Qt::white));	
			item->setText("---");
		}else{		
			item->setBackground(QBrush(QColor(255,192,203)));	
			item->setText("X");
		}
		mask >>= 1;
	}

	//插入传感器数目
	item = mainTableWidget->item(index - 1, 1);
	item->setText(QString::number(sensors, 10));

	//插入note
	item = mainTableWidget->item(index - 1, 7);
	item->setText(Settings::readNote(index));
}

/*==================================================================    
 *    功能：   初始化tableWidget
 *    输入参数：
 *    返回值： 
/*==================================================================*/
void MainWindow::initTableWidget()
{
	mainTableWidget->setColumnCount(9); 
	mainTableWidget->setRowCount(0); 
	mainTableWidget->horizontalHeader()->setStretchLastSection(true);  
	mainTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); 
	mainTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);  
	//mainTableWidget->verticalHeader()->setVisible(false); 

	QStringList headers; 
	headers << QString::fromWCharArray(L"采集站名") <<QString::fromWCharArray(L"传感器数") << 
		QString::fromWCharArray(L"倾角值") << QString::fromWCharArray(L"深层位移") <<QString::fromWCharArray(L"浅表位移")
		<<QString::fromWCharArray(L"雨量值")<<QString::fromWCharArray(L"应力值")
		<<QString::fromWCharArray(L"备注")<<QString::fromWCharArray(L"预警"); 
	mainTableWidget->setHorizontalHeaderLabels(headers); 

	initTableWidgetData();
}


/*==================================================================    
 *    功能    ：   读取ini文件并初始化tableWidget
 *    输入参数：
 *    返回值    ： 
 *    作者    ：   
 *    日期    ：
/*==================================================================*/
void MainWindow::initTableWidgetData()
{
	QString note;
	QTableWidgetItem *item;

	int stations = Settings::readStationNum();
	
	for(int i = mainTableWidget->rowCount(); i < stations; ++i) {
		mainTableWidget->insertRow(i);
		item = new QTableWidgetItem(QString::fromWCharArray(L"采集站 #") + QString::number(i + 1, 10));
		item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		item->setBackground(QBrush(Qt::white));
		mainTableWidget->setItem(i, 0, item); 

		unsigned int mask = Settings::readMask(i + 1);
		int sensors = 0;
		for(int j = 2;  j  <= SENSOR_TYPES + 1; ++j) {
			item = new QTableWidgetItem("---");
			//item = new QTableWidgetItem(QString::number(i , 10)+" A "+QString::number(j, 10));
			item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
			if(mask & 0x01) {
				++sensors;		
				item->setBackground(QBrush(Qt::white));
			}else{		
				item->setBackground(QBrush(QColor(255,192,203)));	
				item->setText("X");
			}
			mainTableWidget->setItem(i, j, item); 
			mask >>= 1;
		}

		//插入传感器数目
		item = new QTableWidgetItem(QString::number(sensors, 10));
		item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		item->setBackground(QBrush(Qt::white));
		mainTableWidget->setItem(i, 1, item); 

		//插入note
		note = Settings::readNote(i + 1);
		item = new QTableWidgetItem(note);
		item->setBackground(QBrush(Qt::white));
		item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		mainTableWidget->setItem(i, 7, item); 

		//插入状态
		item = new QTableWidgetItem();
		item->setBackground(QBrush(Qt::green));
		mainTableWidget->setItem(i, 8, item); 
	}
}

MainWindow::~MainWindow()
{

}

void MainWindow::initTreeWidget()
{
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
	itemConfig->setExpanded(true);

	QTreeWidgetItem *itemAlgorithm = new QTreeWidgetItem(QStringList(QString::fromWCharArray(L"数据处理")));
	itemAlgorithm->setIcon(0, QIcon("./img/04.png"));
	mainTreeWidget->addTreeLeaf(itemAlgorithm,QString::fromWCharArray(L"数据处理模型"), TreeWidget::ALGORITHM1);
	mainTreeWidget->addTreeLeaf(itemAlgorithm,QString::fromWCharArray(L"数据处理算法"), TreeWidget::ALGORITHM1);
	mainTreeWidget->addTopLevelItem(itemAlgorithm);
	itemAlgorithm->setExpanded(true);

	QTreeWidgetItem *help = new QTreeWidgetItem(QStringList(QString::fromWCharArray(L"关于与帮助")));
	help->setIcon(0, QIcon("./img/help.png"));
	mainTreeWidget->addTreeLeaf(help,QString::fromWCharArray(L"关于"), TreeWidget::ABOUT);
	mainTreeWidget->addTreeLeaf(help,QString::fromWCharArray(L"帮助"), TreeWidget::HELP);
	mainTreeWidget->addTopLevelItem(help);
	help->setExpanded(true);

	connect(mainTreeWidget,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),this,SLOT(acceptFromTreeWidget(QTreeWidgetItem*,int)));
}
void MainWindow::updateLeds()
{
	leds->incIndex();
}