#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include "ui_mainwindow.h"


class QHBoxLayout;
class QDockWidget;
class TreeWidget;
class QTreeWidgetItem;
class QTableWidget;
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
	QTableWidget *mainTableWidget;
	void initTreeWidget();
	void initTableWidget();
	
};

#endif // MAINWINDOW_H
