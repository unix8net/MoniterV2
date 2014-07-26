#ifndef DATASOURCE_H
#define DATASOURCE_H

#include <QWidget>
#include "ui_datasource.h"

class DataSource : public QWidget
{
	Q_OBJECT

public:
	DataSource(QWidget *parent = 0);
	~DataSource();
	void closeEvent(QCloseEvent *);
private:
	Ui::DataSource ui;
private Q_SLOTS:
	void ok();
};

#endif // DATASOURCE_H
