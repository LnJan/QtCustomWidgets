#include "QCardDialog.h"
#include <QPainter>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QtMath>
#include <QDebug>

QCardDialog::QCardDialog(QWidget* parent):QDialog(parent),
    m_bIsCloseIconShow(false),m_nRectangleRadius(30)
{
    setWindowFlags(windowFlags()|Qt::FramelessWindowHint|Qt::Popup);
    setAttribute(Qt::WA_TranslucentBackground);
    resize(400,300);
    m_backgroundColor.setRgb(255,255,255);
    m_bIsWindowMoveFlag=false;
}

QCardDialog::~QCardDialog()
{

}

void QCardDialog::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);
    QPen emptyPen(Qt::NoPen);
    QBrush emptyBrush(Qt::NoBrush);

    QRect dialogRect=rect();
    dialogRect.setWidth(dialogRect.width()*0.95);
    dialogRect.setHeight(dialogRect.height()*0.95);

    if(dialogRect.isValid())
    {
        painter.setBrush(emptyBrush);
        QColor clr(0,0,0,50);
        for(int i=0;i<10;i++)
        {
            QPainterPath path;
            path.setFillRule(Qt::WindingFill);
            path.addRoundedRect(i+2,i+2,dialogRect.width(),dialogRect.height(),m_nRectangleRadius,m_nRectangleRadius);
            clr.setAlpha(150-qSqrt(i)*50);
            painter.setPen(clr);
            painter.drawPath(path);
        }

        QBrush backgroundBrush(m_backgroundColor);
        painter.setBrush(backgroundBrush);
        painter.setPen(emptyPen);
        painter.drawRoundedRect(dialogRect,m_nRectangleRadius,m_nRectangleRadius);

        if(m_bIsCloseIconShow)
        {
            if(!m_btnCloseRegion.isEmpty())
            {
                painter.save();
                QRect rect=m_btnCloseRegion.boundingRect();
                painter.translate(rect.x(),rect.y());
                QPen blackPen(QBrush(QColor(30,30,30)),3,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin);
                painter.setBrush(emptyBrush);
                painter.setPen(blackPen);
                painter.drawLine(1,1,rect.width()-1,rect.height()-1);
                painter.drawLine(1,rect.height()-1,rect.width()-1,1);
                painter.restore();
            }
        }
    }
}

void QCardDialog::enterEvent(QEvent*)
{
    m_bIsCloseIconShow=true;
    update();
}

void QCardDialog::leaveEvent(QEvent*)
{
    m_bIsCloseIconShow=false;
    update();
}

void QCardDialog::mouseReleaseEvent(QMouseEvent* e)
{
    m_bIsWindowMoveFlag=false;
    if(!m_btnCloseRegion.isEmpty() &&
            m_btnCloseRegion.contains(e->pos()))
    {
        this->done(QDialog::Accepted);
        //this->finished(QDialog::Accepted);
    }
}

void QCardDialog::resizeEvent(QResizeEvent* e)
{
    QSize newSize=e->size();
    int nIconSize=static_cast<int>(qMin(newSize.width()*0.04,newSize.height()*0.04));
    if(nIconSize>10)
    {
        int nStartX=static_cast<int>(newSize.width()-nIconSize-newSize.width()*0.1);
        int nStartY=static_cast<int>(newSize.height()*0.05);
        m_btnCloseRegion=QRegion(nStartX,nStartY,nIconSize,nIconSize);
    }
    else
    {
        m_btnCloseRegion=QRegion();
    }
}

void QCardDialog::mouseMoveEvent(QMouseEvent* event)
{
    if(m_bIsWindowMoveFlag)
    {
        QPoint ptCurrentGlobal=event->globalPos();
        int moveX=ptCurrentGlobal.x()-m_ptMousePress.x();
        int moveY=ptCurrentGlobal.y()-m_ptMousePress.y();
        //setGeometry(QRect(currenRect.x()+moveX,currenRect.y()+moveY,currenRect.width(),currenRect.height()));
        move(x()+moveX,y()+moveY);
        m_ptMousePress=ptCurrentGlobal;
    }
    QDialog::mouseMoveEvent(event);
}

void QCardDialog::mousePressEvent(QMouseEvent* event)
{
    if(event->button()==Qt::LeftButton &&
            event->pos().y()<=size().height()*0.15)
    {
        m_ptMousePress=event->globalPos();
        m_bIsWindowMoveFlag=true;
    }
    else
    {
        m_bIsWindowMoveFlag=false;
    }
    QDialog::mousePressEvent(event);
}
