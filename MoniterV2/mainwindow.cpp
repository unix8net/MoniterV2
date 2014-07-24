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
	mainDock=new QDockWidget(QString::fromWCharArray(L"�����Ԥ��ϵͳѡ��"),this);
	mainDock->setFeatures(QDockWidget::DockWidgetMovable);  
	mainDock->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);  
	mainTreeWidget = new TreeWidget(mainDock);

	QTreeWidgetItem *itemCollectionStation = new QTreeWidgetItem(QStringList(QString::fromWCharArray(L"�ɼ�վ")),ItemType1);
	itemCollectionStation->setIcon(0, QIcon("./img/01.png"));
	mainTreeWidget->addTreeLeaf(itemCollectionStation,QString::fromWCharArray(L"�ɼ�վ #1"), ItemType2);
	mainTreeWidget->addTreeLeaf(itemCollectionStation,QString::fromWCharArray(L"�ɼ�վ #2"), ItemType2);
	mainTreeWidget->addTreeLeaf(itemCollectionStation,QString::fromWCharArray(L"�ɼ�վ #3"), ItemType2);
	mainTreeWidget->addTreeLeaf(itemCollectionStation,QString::fromWCharArray(L"���Ӳɼ�վ"), ItemType3);
	mainTreeWidget->addTopLevelItem(itemCollectionStation);
	itemCollectionStation->setExpanded(true);

	


	QTreeWidgetItem *itemDataDisplay = new QTreeWidgetItem(QStringList(QString::fromWCharArray(L"������ʾ")),ItemType1);
	itemDataDisplay->setIcon(0, QIcon("./img/02.png"));
	mainTreeWidget->addTreeLeaf(itemDataDisplay,QString::fromWCharArray(L"���ݶ�̬��ʾ"), ItemType2);
	mainTreeWidget->addTreeLeaf(itemDataDisplay,QString::fromWCharArray(L"����ʵʱ����"), ItemType2);
	mainTreeWidget->addTreeLeaf(itemDataDisplay,QString::fromWCharArray(L"��ʷ���ݲ�ѯ"), ItemType2);
	mainTreeWidget->addTreeLeaf(itemDataDisplay,QString::fromWCharArray(L"��ʷ���ݲ�ѯ"), ItemType2);
	mainTreeWidget->addTopLevelItem(itemDataDisplay);
	itemDataDisplay->setExpanded(true);



	QTreeWidgetItem *itemConfig = new QTreeWidgetItem(QStringList(QString::fromWCharArray(L"ϵͳ����")),ItemType1);
	itemConfig->setIcon(0, QIcon("./img/03.png"));
	mainTreeWidget->addTreeLeaf(itemConfig,QString::fromWCharArray(L"������Դ����"), ItemType2);
	mainTreeWidget->addTreeLeaf(itemConfig,QString::fromWCharArray(L"��������"), ItemType2);
	mainTreeWidget->addTreeLeaf(itemConfig,QString::fromWCharArray(L"��������"), ItemType2);
	mainTreeWidget->addTreeLeaf(itemConfig,QString::fromWCharArray(L"���ݿ�����"), ItemType2);
	mainTreeWidget->addTreeLeaf(itemConfig,QString::fromWCharArray(L"�����ļ�����"), ItemType2);
	mainTreeWidget->addTreeLeaf(itemConfig,QString::fromWCharArray(L"��ֵ����"), ItemType2);
	mainTreeWidget->addTopLevelItem(itemConfig);

	QTreeWidgetItem *itemAlgorithm = new QTreeWidgetItem(QStringList(QString::fromWCharArray(L"���ݴ���")),ItemType1);
	itemAlgorithm->setIcon(0, QIcon("./img/04.png"));
	mainTreeWidget->addTreeLeaf(itemAlgorithm,QString::fromWCharArray(L"���ݴ���ģ��"), ItemType2);
	mainTreeWidget->addTreeLeaf(itemAlgorithm,QString::fromWCharArray(L"���ݴ����㷨"), ItemType2);
	mainTreeWidget->addTopLevelItem(itemAlgorithm);

	QTreeWidgetItem *help = new QTreeWidgetItem(QStringList(QString::fromWCharArray(L"���������")),ItemType1);
	help->setIcon(0, QIcon("./img/help.png"));
	mainTreeWidget->addTreeLeaf(help,QString::fromWCharArray(L"����"), ItemType2);
	mainTreeWidget->addTreeLeaf(help,QString::fromWCharArray(L"����"), ItemType2);
	mainTreeWidget->addTopLevelItem(help);

	mainDock->setWidget(mainTreeWidget);  
	addDockWidget(Qt::LeftDockWidgetArea,mainDock);  
}
MainWindow::~MainWindow()
{

}
