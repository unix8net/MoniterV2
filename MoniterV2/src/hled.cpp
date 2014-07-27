#include <QtGui>
#include <QPointF>
#include <QDebug>
#include "../include/hled.h"

struct HLed::Private
{
    Private(): color(Qt::green), lightState(HLed::OFF), haveText(false){}
    QColor color;
    LightState lightState;

    QString labelText;
    bool haveText;
};

void HLed::setHaveText(bool b)
{
    m_d->haveText = b;
}

void HLed::setText(QString s)
{
    m_d->labelText = s;
}

HLed::HLed(QWidget *parent)
    :QWidget(parent), m_d(new Private)
{
}

HLed::~HLed()
{
    delete m_d;
}

QColor HLed::color() const
{
    return m_d->color;
}

void HLed::setColor(const QColor &color)
{
    if (m_d->color == color)
        return;
    else
        m_d->color = color;
}

QSize HLed::sizeHint() const
{
    return QSize(18, 18);
}

QSize HLed::minimumSizeHint() const
{
    return QSize(10, 10);
}


void HLed::turnOn()
{
    setColor(Qt::green);
    m_d->lightState = HLed::ON;
    update();
}

void HLed::haveError()
{
    setColor(Qt::red);
    m_d->lightState = HLed::ERR;
    update();
}

void HLed::turnOff()
{
    setColor(Qt::gray);
    m_d->lightState = HLed::OFF;
    update();
}

void HLed::paintEvent(QPaintEvent*)
{
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setBrush(QBrush(m_d->color));
        painter.drawEllipse(1, 1, 15, 15);
        if(m_d->haveText)
        {
            QFont font;
            font.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
            font.setPointSize(10);
            painter.setFont(font);
            painter.drawText(QPointF(ledWidth(),ledWidth()-6), m_d->labelText);
        }
}

int HLed::ledWidth() const
{
    int width = qMin(this->width(), this->height());
    width += 2;
    return width > 0 ? width : 0;
}

