#include <QtGui>
#include <QPointF>
#include <QDebug>
#include "../include/hleds.h"

HLeds::HLeds(QWidget *parent)
    :QWidget(parent)
{
  //  resize(15,15);
    for(int i = 0; i < 5; ++i)
        bad[i] = false;
    index = 0;
}


QSize HLeds::sizeHint() const
{
    return QSize(18, 18);
}

QSize HLeds::minimumSizeHint() const
{
    return QSize(10, 10);
}




void HLeds::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    for(int i = 0; i < 5; ++i)
    {
        if(bad[i] == true || index == i)
        {
            painter.setBrush(QBrush(Qt::red));
        }
        else
            painter.setBrush(QBrush(Qt::green));
        painter.drawEllipse(1 + 18 * i, 1, 15, 15);
    }
    painter.drawText(1 + 18 * 5, 13, QString::fromWCharArray(L"设备状态"));
}


