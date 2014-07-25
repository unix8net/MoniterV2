#ifndef MODIFYSTATION_H
#define MODIFYSTATION_H

#include <QDialog>
#include "ui_modifystations.h"

class ModifyStation : public QDialog
{
	Q_OBJECT

public:
	ModifyStation(int no = -1, QWidget *parent = 0);
	~ModifyStation();
	void changleTitle(const QString& str)
	{
		(ui.label)->setText(str);
	}
	void readIni(int);
private:
	Ui::ModifyStation ui;
private Q_SLOTS:
	void ok();
};

#endif // MODIFYSTATION_H
