/*********************************************************************************
  * Copyright(C) 2019,保留所有权利。( All rights reserved. )
  * FileName: QGalleryCard.h
  * Author:Ln_Jan
  * Version:v1.0.0
  * Date:2019-05-09
  * Description:画廊卡片样式定义类
  * Others:
  * Function List:
  * History:
  *
**********************************************************************************/
#ifndef QGALLERYCARD_H
#define QGALLERYCARD_H
#include <QRect>
#include <QPixmap>

class QGalleryCard
{
public:
    QGalleryCard(qreal x,qreal y,int nWidth,int nHeight,const QPixmap& pixmap);
    QGalleryCard(qreal x,qreal y,int nWidth,int nHeight);
    virtual ~QGalleryCard();
    void SetCardPosition(const QPointF& ptStart,qreal nWidth,qreal nHeight);
    void SetCardPosition(const QRectF rtCardRegion){m_rtCardRegion=rtCardRegion;}
    void SetCardSize(qreal nWidth,qreal nHeight);
    const QRectF& GetCardRegion() const {return m_rtCardRegion;}
    void SetRoateAngle(double angle){m_dRoateAngle=angle;}
    double GetRoateAngle() const{return m_dRoateAngle;}
    void SetOpacity(int nOpacity){m_nOpacity=nOpacity;}
    int GetOpacity() const {return m_nOpacity;}
    void SetBackgroundPixmap(const QPixmap& pixmap);
    const QPixmap& GetBackgroundPixmap() const {return m_backgroundPixmap;}
    void SetPixmapScaleMode(Qt::AspectRatioMode mode){m_nScaleMode=mode;}
    Qt::AspectRatioMode GetPixmapScaleMode() const {return m_nScaleMode;}
    void MoveTo(const QPointF& ptDest);
    void DrawCard(QPainter& painter);
protected:
    virtual void DrawCardImpl(QPainter& painter);
private:
    QRectF m_rtCardRegion;                               //位置区域
    double m_dRoateAngle;                               //旋转角度
    int m_nOpacity;                                     //透明度
    QPixmap m_backgroundPixmap;                         //背景图片
    Qt::AspectRatioMode m_nScaleMode;                   //缩放类型

    void AssignsPixmap(const QPixmap& pixmap,int nMaxWidth,int nMaxHeight);
    QPixmap CreateDefaultPixmap(int nWidth,int nHeight);
    void ScaleImage(QPixmap& pixmap);
};

#endif // QGALLERYCARD_H
