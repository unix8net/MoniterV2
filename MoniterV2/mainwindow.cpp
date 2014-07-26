#include <QHBoxLayout>
#include <QBrush>

#include "modifystation.h"
#include "mainwindow.h"
#include "../include/settings.h"
#include "../include/global.h"

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
	this->setPalette(palette);

	mainTableWidget = new TableWidget(mainWidget);
	mainTreeWidget = new TreeWidget(mainWidget);
	initTableWidget();
	initTreeWidget();

	hBoxLayout = new QHBoxLayout(mainWidget);
	hBoxLayout->addWidget(mainTableWidget, 8);
	hBoxLayout->addWidget(mainTreeWidget, 1);
	mainWidget->setLayout(hBoxLayout);
}

/*==================================================================    
 *    ���ܣ�   �ú������ڽ���TreeWidget��Item˫���ź�
 *    ���������
 *    ����ֵ�� 
/*==================================================================*/
void MainWindow::acceptFromTreeWidget(QTreeWidgetItem * item, int column)
{
	switch (item->type()) {

		case TreeWidget::STATION:{
			
			//��ȡstation�ı�ţ���Ϊ��ͬ��Station�в�ͬ�ı��
			QString id = item->text(0);
			int index = id.remove(0, id.lastIndexOf("#") + 1).toInt();
			
			ModifyStation *modifyStation = new ModifyStation(index, this);//-1
			modifyStation->changleTitle(QString::fromWCharArray(L"������֪�ɼ�վ��Ϣ"));

			//�ɼ�վ�޸ĺ���OK
			connect(modifyStation, SIGNAL(updateStationSignal(int)), this, SLOT(updateTableItem(int)));
			modifyStation->show();
			break;
		}

		case TreeWidget::STATION_ADD: {
			ModifyStation *modifyStation = new ModifyStation(this);
			modifyStation->changleTitle(QString::fromWCharArray(L"�����²ɼ�վ��Ϣ"));

			//�ɼ�վ������Ϣ��ú���OK
			connect(modifyStation, SIGNAL(addNewStationSignal()), this, SLOT(addNewTableItem()));
			modifyStation->show();
			break;
			}
		}
}

/*==================================================================    
 *    ���ܣ�   ��tableWidget�������µ���
 *    ���������
 *    ����ֵ�� 
/*==================================================================*/
void MainWindow::addNewTableItem()
{
	initTableWidgetData();
}

/*==================================================================    
 *    ���ܣ�   ����tableWidget�е�ĳ��
 *    ���������indexΪ�ɼ�վ��ţ���1��ʼ���
 *    ����ֵ�� 
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
		}else{		
			item->setBackground(QBrush(QColor(35,202,255)));	
			item->setText("---");
		}
		mask >>= 1;
	}

	//���봫������Ŀ
	item = mainTableWidget->item(index - 1, 1);
	item->setText(QString::number(sensors, 10));

	//����note
	item = mainTableWidget->item(index - 1, 8);
	item->setText(Settings::readNote(index));
}

/*==================================================================    
 *    ���ܣ�   ��ʼ��tableWidget
 *    ���������
 *    ����ֵ�� 
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
	headers << QString::fromWCharArray(L"�ɼ�վ��") <<QString::fromWCharArray(L"��������") << 
		QString::fromWCharArray(L"���ֵ") << QString::fromWCharArray(L"���λ��") <<QString::fromWCharArray(L"ǳ��λ��")
		<<QString::fromWCharArray(L"����ֵ")<<QString::fromWCharArray(L"Ӧ��ֵ")
		<<QString::fromWCharArray(L"״̬")<<QString::fromWCharArray(L"��ע"); 
	mainTableWidget->setHorizontalHeaderLabels(headers); 

	initTableWidgetData();
}


/*==================================================================    
 *    ����    ��   ��ȡini�ļ�����ʼ��tableWidget
 *    ���������
 *    ����ֵ    �� 
 *    ����    ��   
 *    ����    ��
/*==================================================================*/
void MainWindow::initTableWidgetData()
{
	QString note;
	QTableWidgetItem *item;

	int stations = Settings::readStationNum();
	
	for(int i = mainTableWidget->rowCount(); i < stations; ++i) {
		mainTableWidget->insertRow(i);
		item = new QTableWidgetItem(QString::fromWCharArray(L"�ɼ�վ #") + QString::number(i + 1, 10));
		item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		mainTableWidget->setItem(i, 0, item); 

		unsigned int mask = Settings::readMask(i + 1);
		int sensors = 0;
		for(int j = 2;  j  <= SENSOR_TYPES + 1; ++j) {
			item = new QTableWidgetItem("---");
			//item = new QTableWidgetItem(QString::number(i , 10)+" A "+QString::number(j, 10));
			item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
			if(mask & 0x01) {
				++sensors;		
			}else{		
				item->setBackground(QBrush(QColor(35,202,255)));			
			}
			mainTableWidget->setItem(i, j, item); 
			mask >>= 1;
		}

		//���봫������Ŀ
		item = new QTableWidgetItem(QString::number(sensors, 10));
		item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		mainTableWidget->setItem(i, 1, item); 

		//����note
		note = Settings::readNote(i + 1);
		item = new QTableWidgetItem(note);
		item->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		mainTableWidget->setItem(i, 8, item); 

		//����״̬
		item = new QTableWidgetItem();
		item->setBackground(QBrush(Qt::green));
		mainTableWidget->setItem(i, 7, item); 
	}
}

MainWindow::~MainWindow()
{

}

void MainWindow::initTreeWidget()
{
	QTreeWidgetItem *itemCollectionStation = new QTreeWidgetItem(QStringList(QString::fromWCharArray(L"�ɼ�վ")));
	itemCollectionStation->setIcon(0, QIcon("./img/01.png"));
	mainTreeWidget->addTreeLeaf(itemCollectionStation,QString::fromWCharArray(L"�ɼ�վ #1"), TreeWidget::STATION);
	mainTreeWidget->addTreeLeaf(itemCollectionStation,QString::fromWCharArray(L"�ɼ�վ #2"), TreeWidget::STATION);
	mainTreeWidget->addTreeLeaf(itemCollectionStation,QString::fromWCharArray(L"�ɼ�վ #3"), TreeWidget::STATION);
	mainTreeWidget->addTreeLeaf(itemCollectionStation,QString::fromWCharArray(L"���Ӳɼ�վ"), TreeWidget::STATION_ADD);
	mainTreeWidget->addTopLevelItem(itemCollectionStation);
	itemCollectionStation->setExpanded(true);

	QTreeWidgetItem *itemDataDisplay = new QTreeWidgetItem(QStringList(QString::fromWCharArray(L"������ʾ")));
	itemDataDisplay->setIcon(0, QIcon("./img/02.png"));
	mainTreeWidget->addTreeLeaf(itemDataDisplay,QString::fromWCharArray(L"���ݶ�̬��ʾ"), TreeWidget::DATA_DYNAMIC_DISPLAY);
	mainTreeWidget->addTreeLeaf(itemDataDisplay,QString::fromWCharArray(L"����ʵʱ����"), TreeWidget::DATA_REALTIME_WAVE);
	mainTreeWidget->addTreeLeaf(itemDataDisplay,QString::fromWCharArray(L"��ʷ���ݲ���"), TreeWidget::DATA_HISTORY_WAVE);
	mainTreeWidget->addTreeLeaf(itemDataDisplay,QString::fromWCharArray(L"��ʷ���ݲ�ѯ"), TreeWidget::DATA_QUERY);
	mainTreeWidget->addTopLevelItem(itemDataDisplay);
	itemDataDisplay->setExpanded(true);

	QTreeWidgetItem *itemConfig = new QTreeWidgetItem(QStringList(QString::fromWCharArray(L"ϵͳ����")));
	itemConfig->setIcon(0, QIcon("./img/03.png"));
	mainTreeWidget->addTreeLeaf(itemConfig,QString::fromWCharArray(L"������Դ����"), TreeWidget::DATA_SOURCE);
	mainTreeWidget->addTreeLeaf(itemConfig,QString::fromWCharArray(L"��������"), TreeWidget::CFG_UART);
	mainTreeWidget->addTreeLeaf(itemConfig,QString::fromWCharArray(L"��������"), TreeWidget::CFG_NET);
	mainTreeWidget->addTreeLeaf(itemConfig,QString::fromWCharArray(L"���ݿ�����"), TreeWidget::CFG_SQL);
	mainTreeWidget->addTreeLeaf(itemConfig,QString::fromWCharArray(L"�����ļ�����"), TreeWidget::CFG_FILE);
	mainTreeWidget->addTreeLeaf(itemConfig,QString::fromWCharArray(L"��ֵ����"), TreeWidget::CFG_LEVEL);
	mainTreeWidget->addTopLevelItem(itemConfig);

	QTreeWidgetItem *itemAlgorithm = new QTreeWidgetItem(QStringList(QString::fromWCharArray(L"���ݴ���")));
	itemAlgorithm->setIcon(0, QIcon("./img/04.png"));
	mainTreeWidget->addTreeLeaf(itemAlgorithm,QString::fromWCharArray(L"���ݴ���ģ��"), TreeWidget::ALGORITHM1);
	mainTreeWidget->addTreeLeaf(itemAlgorithm,QString::fromWCharArray(L"���ݴ����㷨"), TreeWidget::ALGORITHM1);
	mainTreeWidget->addTopLevelItem(itemAlgorithm);

	QTreeWidgetItem *help = new QTreeWidgetItem(QStringList(QString::fromWCharArray(L"���������")));
	help->setIcon(0, QIcon("./img/help.png"));
	mainTreeWidget->addTreeLeaf(help,QString::fromWCharArray(L"����"), TreeWidget::ABOUT);
	mainTreeWidget->addTreeLeaf(help,QString::fromWCharArray(L"����"), TreeWidget::HELP);
	mainTreeWidget->addTopLevelItem(help);

	connect(mainTreeWidget,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),this,SLOT(acceptFromTreeWidget(QTreeWidgetItem*,int)));
}
