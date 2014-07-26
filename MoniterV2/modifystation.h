#ifndef MODIFYSTATION_H
#define MODIFYSTATION_H

#include <QDialog>
#include "ui_modifystations.h"

class ModifyStation : public QDialog
{
	Q_OBJECT

public:
	//是增加页面，还是修改页面
	enum LeafTypes{STATION, STATION_ADD};

	//构造修改配置页面
	ModifyStation(int no, QWidget *parent = 0);
	//构造增加新采集站页面
	ModifyStation( QWidget *parent = 0);
	~ModifyStation();

	//读取指定编号采集站ini配置
	void readIni(int no);

	//更改标题
	void changleTitle(const QString& str)
	{
		(ui.label)->setText(str);
	}
	void closeEvent(QCloseEvent *);

private:
	LeafTypes type;
	int index;
	Ui::ModifyStation ui;

	void checkMask(int no);
	void initModifyStation(int no);

private Q_SLOTS:
	void ok();

Q_SIGNALS:
	void addNewStationSignal();
	void updateStationSignal(int);
};

#endif // MODIFYSTATION_H
