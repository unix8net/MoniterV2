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
class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

private:
	Ui::MainWindowClass ui;
	QHBoxLayout *hBoxLayout;
	QDockWidget *mainDock;
	TreeWidget *mainTreeWidget;
	TableWidget *mainTableWidget;
	void initTreeWidget();
	void initTableWidget();
private Q_SLOTS:
	void acceptFromTreeWidget(QTreeWidgetItem * item, int column);
};

#endif // MAINWINDOW_H
