#ifndef VIEW_H
#define VIEW_H
#include <QTableView>
#include <QTableWidget>
#include <QTreeWidget>
#include <QMenu>
#include <QDebug>
class TableView:public QTableView
{
public:
	TableView(QWidget * parent=0);
	virtual ~ TableView(){}

protected:
	void mouseMoveEvent(QMouseEvent * event);

private:		
};



class TableWidget:public QTableWidget
{
public:
	TableWidget(QWidget * parent=0);
	virtual ~ TableWidget(){}

protected:
	void mouseMoveEvent(QMouseEvent * event);

private:		
};




class TreeWidget : public QTreeWidget
{
	Q_OBJECT

public:
	enum ItemType
	{
		STATION = QTreeWidgetItem::UserType + 1, STATION_ADD,
		DATA_QUERY,DATA_DYNAMIC_DISPLAY, DATA_REALTIME_WAVE, DATA_HISTORY_WAVE,
		DATA_SOURCE, CFG_GLOBAL, CFG_UART, CFG_NET, CFG_SQL,CFG_FILE, CFG_RESET,CFG_LEVEL,
		ALGORITHM1,
		HELP, ABOUT

	};
	TreeWidget(QWidget * parent=0);
	private slots:
		void onCustomContextMenuRequested(const QPoint& pos) {
			QTreeWidgetItem* item = itemAt(pos);

			if (item) {
				showContextMenu(item, viewport()->mapToGlobal(pos));
			}
		}

		void showContextMenu(QTreeWidgetItem* item, const QPoint& globalPos) {
			QMenu menu;
			switch (item->type()) {
			case STATION:
				menu.addAction("This is a STATION");
				break;
			default:break;
			}

			menu.exec(globalPos);
		}

public:
		void addTreeLeaf(QTreeWidgetItem* item, const QString& txt, const int type){
			
			QTreeWidgetItem *itemLeaf = new QTreeWidgetItem(QStringList(txt),type);
			if(type == STATION_ADD)
			{
				itemLeaf->setIcon(0, QIcon("./img/07.png"));
			}
			else
				itemLeaf->setIcon(0, QIcon("./img/05.png"));
			item->addChild(itemLeaf);
			
		}

Q_SIGNALS:
		void sendToMainwindow(const ItemType &type, int no);
};
#endif