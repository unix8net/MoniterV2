#ifndef NETCONFIG_H
#define NETCONFIG_H

#include <QWidget>
#include "ui_netconfig.h"

class NetConfig : public QWidget
{
	Q_OBJECT

public:
	NetConfig(QWidget *parent = 0);
	~NetConfig();

private:
	Ui::NetConfig ui;
};

#endif // NETCONFIG_H
