#ifndef MODIFYSTATION_H
#define MODIFYSTATION_H

#include <QDialog>
#include "ui_modifystations.h"

class ModifyStation : public QDialog
{
	Q_OBJECT

public:
	//������ҳ�棬�����޸�ҳ��
	enum LeafTypes{STATION, STATION_ADD};

	//�����޸�����ҳ��
	ModifyStation(int no, QWidget *parent = 0);
	//���������²ɼ�վҳ��
	ModifyStation( QWidget *parent = 0);
	~ModifyStation();

	//��ȡָ����Ųɼ�վini����
	void readIni(int no);

	//���ı���
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
