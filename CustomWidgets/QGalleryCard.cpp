#include "QGalleryCard.h"
#include <QPainter>
#include <QLinearGradient>

QGalleryCard::QGalleryCard(qreal x,qreal y,int nWidth,int nHeight,const QPixmap& pixmap)
{
    m_rtCardRegion.setRect(x,y,nWidth,nHeight);
    m_dRoateAngle=0;
    m_nOpacity=255;
    m_nScaleMode=Qt::AspectRatioMode::IgnoreAspectRatio;
    if(pixmap.isNull())
    {
        m_backgroundPixmap=CreateDefaultPixmap(nWidth,nHeight);
    }
    else
    {
        AssignsPixmap(pixmap,m_rtCardRegion.width(),m_rtCardRegion.height());
    }
}

QGalleryCard::QGalleryCard(qreal x,qreal y,int nWidth,int nHeight)
{
    m_rtCardRegion.setRect(x,y,nWidth,nHeight);
    m_dRoateAngle=0;
    m_nOpacity=255;
    m_nScaleMode=Qt::AspectRatioMode::IgnoreAspectRatio;
    m_backgroundPixmap=CreateDefaultPixmap(nWidth,nHeight);
}

QGalleryCard::~QGalleryCard()
{

}

void QGalleryCard::SetCardPosition(const QPointF& ptStart,qreal nWidth,qreal nHeight)
{
    m_rtCardRegion.setTopLeft(ptStart);
    m_rtCardRegion.setWidth(nWidth);
    m_rtCardRegion.setHeight(nHeight);
    ScaleImage(m_backgroundPixmap);
}

void QGalleryCard::SetCardSize(qreal nWidth,qreal nHeight)
{
    m_rtCardRegion.setWidth(nWidth);
    m_rtCardRegion.setHeight(nHeight);
    ScaleImage(m_backgroundPixmap);
}

void QGalleryCard::MoveTo(const QPointF& ptDest)
{
    m_rtCardRegion.moveTo(ptDest);
}

void QGalleryCard::SetBackgroundPixmap(const QPixmap& pixmap)
{
    if(pixmap.isNull())
    {
        m_backgroundPixmap=CreateDefaultPixmap(m_rtCardRegion.width(),m_rtCardRegion.height());
    }
    else
    {
        AssignsPixmap(pixmap,m_rtCardRegion.width(),m_rtCardRegion.height());
    }
}

void QGalleryCard::AssignsPixmap(const QPixmap& pixmap,int nMaxWidth,int nMaxHeight)
{
    if(pixmap.width()>nMaxWidth || pixmap.height()>nMaxHeight)
    {
        m_backgroundPixmap=pixmap.scaled(nMaxWidth,nMaxHeight,m_nScaleMode,Qt::TransformationMode::SmoothTransformation);
    }
    else
    {
        m_backgroundPixmap=pixmap;
    }
}

void QGalleryCard::DrawCard(QPainter& painter)
{
    DrawCardImpl(painter);
}

void QGalleryCard::DrawCardImpl(QPainter& painter)
{
    //painter.save();
    //painter.translate(m_rtCardRegion.topLeft());
    qreal nStartX=(m_rtCardRegion.width()-m_backgroundPixmap.width())/2+m_rtCardRegion.left();
    qreal nStartY=m_rtCardRegion.height()-m_backgroundPixmap.height()+m_rtCardRegion.top();
    painter.setOpacity(static_cast<double>(m_nOpacity)/255);
    QTransform transform;
    //transform.translate(m_rtCardRegion.left(),m_rtCardRegion.top());
    transform.translate(nStartX,nStartY);
    transform.rotate(m_dRoateAngle,Qt::Axis::YAxis);
    painter.setTransform(transform);
    painter.drawPixmap(0,0,m_backgroundPixmap);

    //画倒影
    painter.setOpacity(0.2);
    transform.scale(1,-1);
    painter.setTransform(transform);
    painter.drawPixmap(0,static_cast<int>(-m_backgroundPixmap.height()*2.05),m_backgroundPixmap);
    painter.resetTransform();
    //painter.restore();
}

void QGalleryCard::ScaleImage(QPixmap& pixmap)
{
    if(pixmap.isNull())
    {
        pixmap=CreateDefaultPixmap(m_rtCardRegion.width(),m_rtCardRegion.height());
    }
    else
    {
        pixmap=pixmap.scaled(m_rtCardRegion.width(),m_rtCardRegion.height(),m_nScaleMode,Qt::TransformationMode::SmoothTransformation);
    }
}

QPixmap QGalleryCard::CreateDefaultPixmap(int nWidth,int nHeight)
{
    QPixmap defaultPixmap(nWidth,nHeight);
    QPainter painter;
    if(painter.begin(&defaultPixmap))
    {
        QLinearGradient gradient(0,0,nWidth*3/4,nHeight);
        gradient.setColorAt(0,QColor(Qt::GlobalColor::black));
        gradient.setColorAt(1,QColor(Qt::GlobalColor::white));
        QBrush brush(gradient);
        painter.setBrush(brush);
        painter.fillRect(0,0,nWidth,nHeight,brush);
        painter.end();
    }
    return defaultPixmap;
}



