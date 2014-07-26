#ifndef UARTCONFIG_H
#define UARTCONFIG_H

#include <QWidget>
#include "ui_uartconfig.h"

class QextSerialPort;
class QextSerialEnumerator;
class UartConfig : public QWidget
{
	Q_OBJECT

public:
	UartConfig(QextSerialPort *, QWidget *parent = 0);
	~UartConfig();
	void closeEvent(QCloseEvent *);

private Q_SLOTS:
		void onPortNameChanged(const QString&);
		void onBaudRateChanged(int);
		void onParityChanged(int);
		void onDataBitsChanged(int);
		void onStopBitsChanged(int);
		void onPortAddedOrRemoved();
		void ok();
private:
	Ui::UartConfig *ui;
	QextSerialPort *port;
	QextSerialEnumerator *enumerator;
};

#endif // UARTCONFIG_H
