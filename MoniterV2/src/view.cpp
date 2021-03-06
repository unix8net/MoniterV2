#include "../include/view.h"
#include <QHeaderView>
TableView::TableView(QWidget * parent)
	:QTableView(parent)
{
	
	
}

void TableView::mouseMoveEvent(QMouseEvent * event)
{
	QTableView::mouseMoveEvent(event);
}

TableWidget::TableWidget(QWidget * parent)
	:QTableWidget(parent)
{
	setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	verticalHeader()->setVisible(false);
	setAlternatingRowColors(true);
	QPalette pll = this->palette();
	pll.setBrush(QPalette::Base,QBrush(QColor(255,255,255,0)));
	this->setPalette(pll); 
}

void TableWidget::mouseMoveEvent(QMouseEvent * event)
{
	QTableWidget::mouseMoveEvent(event);
}



TreeWidget::TreeWidget(QWidget * parent)	
	:QTreeWidget(parent)
{
	setContextMenuPolicy(Qt::CustomContextMenu);
	setColumnCount(1);
	//setHorizontalScrollBarPolicy (Qt::ScrollBarAsNeeded);
	
	//this->header()->setSectionResizeMode(0, QHeaderView::Stretch);
	this->header()->setVisible(false);
	connect(this,
		SIGNAL(customContextMenuRequested(const QPoint&)),
		SLOT(onCustomContextMenuRequested(const QPoint&)));
	
}
