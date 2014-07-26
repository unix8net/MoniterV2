#include <qwt/qwt_plot_layout.h>
#include <qwt/qwt_plot_curve.h>
#include <qwt/qwt_scale_draw.h>
#include <qwt/qwt_scale_widget.h>
#include <qwt/qwt_plot_canvas.h>
#include <qwt/qwt_symbol.h>
#include <qwt/qwt_painter.h>
#include <qwt/qwt_clipper.h>
#include <qwt/qwt_curve_fitter.h>
#include <qwt/qwt_picker_machine.h>
#include <qwt/qwt_plot_picker.h>
#include <qwt/qwt_plot_grid.h>

#include <QTime>

#include "../include/plot.h"
#include "../src/utility/pointMapper/pointmapper.h"
#include "../src/utility/pointMapper/pointmapper.cpp"
#include "../src/utility/utility.h"
#include "../include/griditem.h"

/*==================================================================
 *    作者：Creater
 *    简介：派生curv管理器QwtPlot
 *    主页： unix8.net
 *    日期：2014-04-22
/*==================================================================*/

/*!
  \breif 重载的坐标刻度Draw
*/
class TimeScaleDraw: public QwtScaleDraw
{
public:
    TimeScaleDraw() {}

    //! [0]
    /*!
      \breif 这个函数是重画坐标轴必须实现的虚函数
    */
    virtual QwtText label( double v ) const
    {
        long intValue = static_cast<long>(v);
        int iHour = intValue / 3600;
        int iMin = (intValue - iHour * 3600) / 60;
        int iSen = intValue - iHour * 3600 - iMin * 60;
        return QString::number(iHour, 10).rightJustified(2,'0') + ":" +
                QString::number(iMin, 10).rightJustified(2,'0') + ":" +
                QString::number(iSen, 10).rightJustified(2,'0');
    }
    //!~ [0]
};

/*!
  \breif QwtPlot管理的项目,用于填充cavas的背景
*/
//背景
class Background: public QwtPlotItem
{
public:
    Background() { setZ( 0.0 );}
    //! [0]
    /*!
      \breif 这个函数是重画坐标轴必须实现的虚函数
    */
    virtual int rtti() const
    {
        return QwtPlotItem::Rtti_PlotUserItem;
    }
    //!~ [0]
    //! [1]
    virtual void draw( QPainter *painter,
        const QwtScaleMap &, const QwtScaleMap &,
        const QRectF &canvasRect ) const
    {
        QColor c( Qt::black );
        QRectF r = canvasRect;
        painter->fillRect( r, c );
    }
    //!~ [1]
};

/*!
  \breif 重载的curv
*/
class MyCurve: public QwtPlotCurve
{
public:
    MyCurve( const QString &title ):
        QwtPlotCurve( title )
    {   //优化显示
        setRenderHint( QwtPlotItem::RenderAntialiased );
    }
    void setColor( const QColor &color )
    {
        QColor c = color;
        c.setAlpha( 150 );
        setPen( c );
        setBrush( c );
    }
protected:
    #undef OVERLOAD_DRAWCURVE
    #ifdef OVERLOAD_DRAWCURVE
    /*!
      \breif 重载的分发函数，由于以下几个draw函数为虚函数，这里就没重载了
    */
    virtual void drawCurve( QPainter *p, int style, const QwtScaleMap &xMap,
                            const QwtScaleMap &yMap, const QRectF &canvasRect, int from, int to ) const
    {
        switch ( style )
        {
            case QwtPlotCurve::UserCurve:
                //drawMyCurve(p,  xMap, yMap, canvasRect, from, to);
                //drawCustomDots(p,  xMap, yMap, canvasRect, from, to);
                break;
            default:
                 QwtPlotCurve::drawCurve(p, style, xMap, yMap,canvasRect, from, to);
                break;
       }
    }
    #endif

    /*!
      \breif 重载的画线函数，对于y小于0的点跳过
    */
    virtual void drawLines( QPainter *painter, const QwtScaleMap &xMap, const QwtScaleMap &yMap,
        const QRectF &canvasRect, int from, int to ) const
    {
        if ( from > to )
            return;
        const bool doAlign = QwtPainter::roundingAlignment( painter );
        const bool doFit = (testCurveAttribute(Fitted) ) && curveFitter();
        const bool doFill = (brush().style() != Qt::NoBrush )
                && ( brush().color().alpha() > 0 );

        QRectF clipRect;
        if (testPaintAttribute(ClipPolygons))
        {
            qreal pw = qMax( qreal( 1.0 ), painter->pen().widthF());
            clipRect = canvasRect.adjusted(-pw, -pw, pw, pw);
        }



        const bool noDuplicates = testPaintAttribute(FilterPoints);
        //QwtPointMapper
        MyPointMapper mapper;
        mapper.setFlag( MyPointMapper::RoundPoints, doAlign );
        mapper.setFlag( MyPointMapper::WeedOutPoints, noDuplicates );
        mapper.setBoundingRect( canvasRect );
        #if 0
        bool doIntegers = false;
        if ( doIntegers )
        {
            const QPolygon polyline = mapper.toPolygon(
                xMap, yMap, data(), from, to );

            if (testPaintAttribute(ClipPolygons))
            {
                const QPolygon clipped = QwtClipper::clipPolygon(
                    clipRect.toAlignedRect(), polyline, false );

                QwtPainter::drawPolyline( painter, clipped );
            }
            else
            {
                QwtPainter::drawPolyline( painter, polyline );
            }
        }
        else
        #endif
        {
            //****
            QPolygonF polyline = mapper.toPolygonF( xMap, yMap, data(), from, to );
            if ( doFit )
                polyline = curveFitter()->fitCurve( polyline );
            if (testPaintAttribute(ClipPolygons))
            {
                const QPolygonF clipped = QwtClipper::clipPolygonF(
                    clipRect, polyline, false );
                QwtPainter::drawPolyline( painter, clipped );
            }
            else
            {
                QwtPainter::drawPolyline( painter, polyline );
            }

            if ( doFill )
            {
                fillCurve( painter, xMap, yMap, canvasRect, polyline );
            }
        }
    }

    /*!
      \breif 重载的画点函数，对于y小于0的点跳过
    */
    virtual void drawDots( QPainter *painter, const QwtScaleMap &xMap, const QwtScaleMap &yMap,
        const QRectF &canvasRect, int from, int to ) const
    {
        const QColor color = painter->pen().color();

        if ( painter->pen().style() == Qt::NoPen || color.alpha() == 0 )
        {
            return;
        }

        const bool doFill = ( brush().style() != Qt::NoBrush )
                && ( brush().color().alpha() > 0 );
        const bool doAlign = QwtPainter::roundingAlignment( painter );

        MyPointMapper mapper;
        mapper.setBoundingRect( canvasRect );
        mapper.setFlag( MyPointMapper::RoundPoints, doAlign );

        if ( testPaintAttribute(FilterPoints))
        {
            if ( ( color.alpha() == 255 )
                && !( painter->renderHints() & QPainter::Antialiasing ) )
            {
                mapper.setFlag( MyPointMapper::WeedOutPoints, true );
            }
        }

        if ( doFill )
        {
            mapper.setFlag( MyPointMapper::WeedOutPoints, false );

            QPolygonF points = mapper.toPointsF(xMap, yMap, data(), from, to );

            QwtPainter::drawPoints( painter, points );
            fillCurve( painter, xMap, yMap, canvasRect, points );
        }
        else if ( testPaintAttribute(ImageBuffer))
        {
            const QImage image = mapper.toImage( xMap, yMap, data(), from, to, QwtPlotCurve::pen(),
                painter->testRenderHint( QPainter::Antialiasing ),
                renderThreadCount() );

            painter->drawImage( canvasRect.toAlignedRect(), image );
        }
        else if ( testPaintAttribute(MinimizeMemory))
        {
            const QwtSeriesData<QPointF> *series = data();

            for ( int i = from; i <= to; i++ )
            {
                const QPointF sample = series->sample( i );

                double xi = xMap.transform( sample.x() );
                double yi = yMap.transform( sample.y() );

                if ( doAlign )
                {
                    xi = qRound( xi );
                    yi = qRound( yi );
                }

                QwtPainter::drawPoint( painter, QPointF( xi, yi ) );
            }
        }
        else
        {
            if ( doAlign )
            {
                const QPolygon points = mapper.toPoints( xMap, yMap, data(), from, to );
                QwtPainter::drawPoints( painter, points );
            }
            else
            {
                const QPolygonF points = mapper.toPointsF( xMap, yMap, data(), from, to );
                QwtPainter::drawPoints( painter, points );
            }
        }
    }
    /*!
      \breif 定制的画非等间隔点函数
    */
    virtual void drawCustomDots(QPainter *p, const QwtScaleMap &xMap,
                           const QwtScaleMap &yMap, const QRectF &, int from, int to) const
    {
        const QwtSeriesData<QPointF> *series = data();
        for (int i = from; i <= to; i++)
        {
            const QPointF sample = series->sample( i );
            double xi = xMap.transform( sample.x() );
            double yi = yMap.transform( sample.y() );
            //qDebug()<< sample.y() <<"   ";
            if(sample.y() < 0)
                continue;
            else
                QwtPainter::drawPoint(p,QPoint(xi, yi));
        }
    }

    /*!
      \breif 定制的非连续函数
    */
    virtual void 	drawCustomLines (QPainter *p, const QwtScaleMap &xMap,
                                const QwtScaleMap &yMap, const QRectF &canvasRect, int from, int to) const
    {
        int preceding_from = from;
        bool is_gap = true;
        const QwtSeriesData<QPointF> *d_series = data();
        for (int i = from; i <= to; i++)
        {
            const QPointF sample = d_series->sample(i);
            if(sample.y() >= 0 && is_gap)
            {
                preceding_from = i;
                is_gap = false;
            }

            if((sample.y() < 0 && !is_gap) || (i == to && sample.y() >= 0))
            {
                drawSteps(p, xMap, yMap, canvasRect, preceding_from, i>from ? i-1 : i); // or drawLines, drawSticks, drawDots
                is_gap = true;
            }
        }
    }

};

//! [0]
Plot::Plot( QWidget *parent ):
    QwtPlot( parent )
{
    //禁止自动replot
    setAutoReplot( false );
    //设置画布
    QwtPlotCanvas *canvas = new QwtPlotCanvas(this);
    canvas->setBorderRadius( 10 );
    setCanvas( canvas );

    plotLayout()->setAlignCanvasToScales( true );

    //设置坐标轴标题
    //setAxisTitle( QwtPlot::xBottom, tr("时间 [h:m:s]") );
    //设置坐标轴字体
    setAxisFont(QwtPlot::xBottom, QFont("Times",8));

    //设置自定义的坐标轴Draw+++
    setAxisScaleDraw( QwtPlot::xBottom,
                      new TimeScaleDraw() );
    setAxisScale( QwtPlot::xBottom, 0, 9, 1 );
    setAxisLabelRotation( QwtPlot::xBottom, -50.0 );
    setAxisLabelAlignment( QwtPlot::xBottom,
                           Qt::AlignLeft | Qt::AlignBottom );

    setAxisTitle( QwtPlot::yLeft, tr("Value [%]") );
    setAxisScale( QwtPlot::yLeft, 0, 100 );

    QwtScaleWidget *scaleWidget = axisWidget( QwtPlot::xBottom );
    const int fmh = QFontMetrics( scaleWidget->font() ).height();
    scaleWidget->setMinBorderDist( 0, fmh / 2 );

   // Background *bg = new Background();
   // bg->attach( this );
    GridItem * grid = new GridItem();
    grid->attach(this);

    //Legend *legend = new Legend(this);
   // insertLegend( legend, QwtPlot::RightLegend );

    curve = new QwtPlotCurve(tr("Today"));
    curve->setPen( QPen(Qt::red, 3 ));
    //设置了该刷子后，将会在线与baseline之间有填充
    //mycurve->setBrush(QBrush(Qt::red));
    curve->setRenderHint( QwtPlotItem::RenderAntialiased );
    curve->setPaintAttribute(QwtPlotCurve::ClipPolygons );
    QwtSymbol *symbol = new QwtSymbol( QwtSymbol::Ellipse,
       QBrush( Qt::yellow ), QPen( Qt::red, 2 ), QSize( 4, 4 ) );
    curve->setSymbol( symbol );

    //mycurve->setStyle( QwtPlotCurve::Dots);
    //mycurve->setStyle( QwtPlotCurve::NoCurve);
    curve->attach( this );


    d_picker = new QwtPlotPicker( QwtPlot::xBottom, QwtPlot::yLeft,
        QwtPlotPicker::CrossRubberBand, QwtPicker::AlwaysOn,
        this->canvas() );
    d_picker->setStateMachine( new QwtPickerDragPointMachine() );;
    d_picker->setRubberBandPen(QColor(160,32,240));
    d_picker->setRubberBand( QwtPicker::CrossRubberBand );
    d_picker->setTrackerPen( QColor( QColor(160,32,240) ) );

    d_grid = new QwtPlotGrid();
    d_grid->attach(this);
    d_grid->setPen(QColor(50, 205, 50), 0.0);
    //d_grid->setPen( QColor(50,205,50),0.0, Qt::DotLine );
    d_grid->setVisible(true);
    d_grid->enableXMin(true);
    d_grid->enableYMin(true);
    //初始化y轴
    data = new double[DISPLAY_POINT_NUM];
    for ( int i = 0; i < DISPLAY_POINT_NUM; ++i )
        data[i] = 0;
    //初始化计数值
    dataCnt = 0;
}
//!~ [0]
Plot::~Plot()
{
    Utility::DESTORY_ARR(data);
    Utility::DESTORY_PTR(d_picker);
    Utility::DESTORY_PTR(d_grid);
    Utility::DESTORY_PTR(curve);
}

//! [1]
/*!
  \breif 画curv，重设坐标轴
*/
void Plot::paintPlotByTime(long t )
{
    setAxisScale( QwtPlot::xBottom, t - 9, t, 1);
    curve->setRawSamples(x, data, dataCnt);
    replot();
}
//!~ [1]

void Plot::setYLabel(const QString& title)
{
    setAxisTitle( QwtPlot::yLeft, title);
}
