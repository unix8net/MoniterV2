#ifndef _PLOT_
#define _PLOT_
#include  <QMutex>
#include  <qwt/qwt_plot.h>


QT_BEGIN_NAMESPACE
class QwtPlotPicker;
class QwtPlotCurve;
class QwtPlotGrid;
QT_END_NAMESPACE

#define DISPLAY_POINT_NUM 11


class Plot : public QwtPlot
{
    Q_OBJECT
public:
    Plot( QWidget * = 0 );
    ~Plot();
    const QwtPlotCurve *getCurve() const{return curve;}
    /*!
     \breif 根据串口数据更新存储
     \param index为索引，f为数据
    */
private Q_SLOTS:
    void setDataByIndex(long , float f)
    {
        mutex.lock();
        for(int i = DISPLAY_POINT_NUM - 1; i > 0; --i)
            data[i] = data[i - 1];
        data[0] = static_cast<double>(f);
        mutex.unlock();

        if(dataCnt < DISPLAY_POINT_NUM)
            ++dataCnt;
    }

    //! [2]
    /*!
      \breif 根据总线更新的时间来推动波形前进
      \param 当前总线时间
    */
public:
    void setIndexByTime(long t)
    {
        if(mutex.tryLock())
        {
            for(int i = DISPLAY_POINT_NUM - 1; i > 0; --i)
                data[i] = data[i-1];
            data[0];
#ifdef DEBUG
            using namespace  std;
            for(int i = DISPLAY_POINT_NUM - 1; i >= 0; --i)
                cout<<data[i]<<"  ";
#endif
            if(dataCnt < DISPLAY_POINT_NUM)
                ++dataCnt;
            paintPlotByTime(t);
            mutex.unlock();
        }
        else
            return;
        for(int j = DISPLAY_POINT_NUM -1; j >= 0; --j)
            x[j] = t - j;
    }

     //!~ [2]
    void paintPlotByTime(long t);
    void setYLabel(const QString& title);

/*!
*/
private:
    int dataCnt;
    //x轴
    double x[DISPLAY_POINT_NUM];
    //放入栈中，有助于提高速度
    QMutex mutex;

    QwtPlotCurve *curve;
    //y值，Warning：数组太大，出现RUNTIMEERROR
    double *data;
    //坐标轴
    QwtPlotPicker *d_picker;
    QwtPlotGrid *d_grid;
};
#endif
