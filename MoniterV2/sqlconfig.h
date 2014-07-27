#ifndef SQLCONFIG_H
#define SQLCONFIG_H

#include <QWidget>
#include "ui_sqlconfig.h"

class DataWidget;
class SqlConfig : public QWidget
{
	Q_OBJECT

public:
	SqlConfig(DataWidget *dw, QWidget *parent = 0);
	~SqlConfig();
	void closeEvent(QCloseEvent *);

private Q_SLOTS:
		void ok();
private:
	Ui::SqlConfig *ui;
	DataWidget *dataWidget;
};

#endif // SQLCONFIG_H
