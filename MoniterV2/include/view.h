#ifndef VIEW_H
#define VIEW_H
#include <QTableView>
#include <QTreeWidget>
#include <QMenu>
class TableView:public QTableView
{
public:
	TableView(QWidget * parent=0);
	virtual ~ TableView(){}

protected:
	void mouseMoveEvent(QMouseEvent * event);

private:		
};



static const int ItemType1 = QTreeWidgetItem::UserType;
static const int ItemType2 = QTreeWidgetItem::UserType + 1;
static const int ItemType3 = QTreeWidgetItem::UserType + 2;
class TreeWidget : public QTreeWidget
{
	Q_OBJECT

public:
	TreeWidget(QWidget * parent=0);
	private slots:
		void onCustomContextMenuRequested(const QPoint& pos) {
			printf("hi\n");
			QTreeWidgetItem* item = itemAt(pos);

			if (item) {
				showContextMenu(item, viewport()->mapToGlobal(pos));
			}
		}

		void showContextMenu(QTreeWidgetItem* item, const QPoint& globalPos) {
			QMenu menu;

			switch (item->type()) {
			case ItemType1:
				menu.addAction("This is a type 1");
				break;

			case ItemType2:
				menu.addAction("This is a type 2");
				break;
			}

			menu.exec(globalPos);
		}
public:
		void addTreeLeaf(QTreeWidgetItem* item, const QString& txt, const int type){
			
			QTreeWidgetItem *itemLeaf = new QTreeWidgetItem(QStringList(txt),type);
			if(type == ItemType3)
			{
				itemLeaf->setIcon(0, QIcon("./img/07.png"));
			}
			else
				itemLeaf->setIcon(0, QIcon("./img/05.png"));
			item->addChild(itemLeaf);
			
		}
};
#endif