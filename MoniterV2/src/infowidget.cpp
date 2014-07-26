#include <QGridLayout>
#include <QLabel>
#include <QFont>
#include <QPalette>
#include "../include/infowidget.h"
#include "utility/utility.h"
/*==================================================================
 *    作者：Creater
 *    简介：该类用于在主面板中显示系统相关信息,包括系统时间，数据库状态，串口状态
 *    主页： unix8.net
 *    日期：2014-04-22
/*==================================================================*/

InfoWidget::InfoWidget(QWidget *parent): QWidget( parent )
{
   gLayout = new QGridLayout(this);

   this->setAutoFillBackground(true);
   QPalette palette;
   palette.setColor(QPalette::Background, QColor(35,202,255));
   this->setPalette(palette);

   //sqlState = new QLabel("MySQL State:Normal", this);
 //  threadState = new QLabel("UART State:Normal",this);

   QFont font;
   font.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
   font.setPointSize(40);
   runtimeLabel = new QLabel("00:00:00", this);
   runtimeLabel->setFont(font);
   runtimeLabel->setTextFormat(Qt::AutoText);
   runtimeLabel->setAlignment(Qt::AlignCenter);
   #if 0
   font.setPointSize(10);
   sqlState->setFont(font);
   sqlState->setTextFormat(Qt::AutoText);
   sqlState->setAlignment(Qt::AlignCenter);
   sqlState->setFrameShape(QFrame::Panel);

   threadState->setFont(font);
   threadState->setTextFormat(Qt::AutoText);
   threadState->setAlignment(Qt::AlignCenter);
   threadState->setFrameShape(QFrame::Panel);
#endif
   int row = 0;
   gLayout->addWidget(runtimeLabel, row++, 0, 1, 2);
#if 0
   gLayout->addWidget(sqlState, row, 0);
   gLayout->addWidget(threadState, row++, 1);
#endif
   //gLayout->setRowStretch( row, 10 );
   setLayout(gLayout);
}

void InfoWidget::setSqlText(QString s)
{
    //sqlState->setText(s);
}

void InfoWidget::setSqlTextPen(QColor color)
{
    QPalette pe;
    pe.setColor(QPalette::WindowText,color);
//    sqlState->setPalette(pe);
}

InfoWidget::~InfoWidget()
{
    Utility::DESTORY_PTR(runtimeLabel);
    Utility::DESTORY_PTR(gLayout);
}

//! [0]
/*!
 \breif 根据信息面板上的系统时间
*/
void InfoWidget::timeBusDispath(long intValue)
{
    int iHour = intValue / 3600;
    int iMin = (intValue - iHour * 3600) / 60;
    int iSen = intValue - iHour * 3600 - iMin * 60;
    runtimeLabel->setText(QString::number(iHour, 10).rightJustified(2,'0') + ":" +
            QString::number(iMin, 10).rightJustified(2,'0') + ":" +
            QString::number(iSen, 10).rightJustified(2,'0'));
}
//!~ [0]

