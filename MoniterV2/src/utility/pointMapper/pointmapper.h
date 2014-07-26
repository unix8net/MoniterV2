#ifndef QWT_POINT_MAPPER_H
#define QWT_POINT_MAPPER_H

#include "qwt/qwt_global.h"
#include "qwt/qwt_series_data.h"
#include <qimage.h>
/*==================================================================
 *    作者：Creater
 *    简介：该类为工具类，为从Qwt中修改而来
 *    主页： unix8.net
 *    日期：2014-04-22
/*==================================================================*/
class QwtScaleMap;
class QPolygonF;
class QPolygon;


class  MyPointMapper
{
public:

    enum TransformationFlag
    {
        //转换为整数值
        RoundPoints = 0x01,
        //移除相邻点的想同值
        WeedOutPoints = 0x02
    };

    typedef QFlags<TransformationFlag> TransformationFlags;

    MyPointMapper();
    ~MyPointMapper();

    void setFlags( TransformationFlags );
    TransformationFlags flags() const;

    void setFlag( TransformationFlag, bool on = true );
    bool testFlag( TransformationFlag ) const;

    void setBoundingRect( const QRectF & );
    QRectF boundingRect() const;

    QPolygonF toPolygonF( const QwtScaleMap &xMap, const QwtScaleMap &yMap,
        const QwtSeriesData<QPointF> *series, int from, int to ) const;

    QPolygon toPolygon( const QwtScaleMap &xMap, const QwtScaleMap &yMap,
        const QwtSeriesData<QPointF> *series, int from, int to ) const;

    QPolygon toPoints( const QwtScaleMap &xMap, const QwtScaleMap &yMap,
        const QwtSeriesData<QPointF> *series, int from, int to ) const;

    QPolygonF toPointsF( const QwtScaleMap &xMap, const QwtScaleMap &yMap,
        const QwtSeriesData<QPointF> *series, int from, int to ) const;

    QImage toImage( const QwtScaleMap &xMap, const QwtScaleMap &yMap,
        const QwtSeriesData<QPointF> *series, int from, int to, 
        const QPen &, bool antialiased, uint numThreads ) const;

private:
    class PrivateData;
    PrivateData *d_data;
};

Q_DECLARE_OPERATORS_FOR_FLAGS( MyPointMapper::TransformationFlags )

#endif
