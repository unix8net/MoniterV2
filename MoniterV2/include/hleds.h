#ifndef HLeds_H
#define HLeds_H

//2014-05-16 该页引起一个异常，需要注意
#include <QWidget>
#include <QString>
QT_BEGIN_NAMESPACE
class QColor;
QT_END_NAMESPACE

class HLeds : public QWidget
{
    Q_OBJECT
public:
    HLeds(QWidget *parent = 0);

    QSize sizeHint() const;
    QSize minimumSizeHint() const;
    void incIndex()
    {
        ++index;
        if(index >= 5)
            index = 0;
        repaint();
    }
    void setBad(int i, bool f = true)
    {
        bad[i] = f;
    }
    void setIndex(int i){index = i;}
protected:
    void paintEvent(QPaintEvent*);

private:
    bool bad[5];
    int index;
};

#endif // HLeds_H
