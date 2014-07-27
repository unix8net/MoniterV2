#ifndef NETCONFIG_H
#define NETCONFIG_H

#include <QWidget>
#include "ui_netconfig.h"

class DataWidget;
class NetConfig : public QWidget
{
	Q_OBJECT

public:
	NetConfig(DataWidget*, QWidget *parent = 0);
	~NetConfig();
	void closeEvent(QCloseEvent *);

private:
	Ui::NetConfig *ui;
	DataWidget *dataWidget;
private Q_SLOTS:
		void ok();
};

#endif // NETCONFIG_H
