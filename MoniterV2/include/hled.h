#ifndef HLED_H
#define HLED_H

#include <QWidget>
#include <QString>

QT_BEGIN_NAMESPACE
class QColor;
QT_END_NAMESPACE

class HLed : public QWidget
{
    Q_OBJECT
public:
    enum LightState{ON, OFF, ERR};
    HLed(QWidget *parent = 0);
    ~HLed();
    QColor color() const;
    QSize sizeHint() const;
    QSize minimumSizeHint() const;
    void setText(QString txt);
    void setHaveText(bool f = false);
public slots:
    void turnOn();
    void turnOff();
    void haveError();
private:
    void setColor(const QColor &color);
protected:
    void paintEvent(QPaintEvent*);
    int ledWidth() const;

private:
    struct Private;
    Private *  m_d;

};

#endif // HLED_H
