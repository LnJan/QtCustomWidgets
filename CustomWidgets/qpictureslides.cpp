#include "qpictureslides.h"
#include "qpixmapobject.h"
#include <QPainter>
#include <QDebug>
#include <QMovie>

QPictureSlides::QPictureSlides(QWidget *parent) : QLabel(parent),m_bIsAutoChange(false),m_bIsScaledPicture(false)
  ,m_nChangeInterval(5000),m_nMouseEnterState(0),m_nCurrentPicIndex(0)
{
    m_nTimerId=startTimer(m_nChangeInterval);
    setMouseTracking(true);
}

QPictureSlides::~QPictureSlides()
{
    ClearAllPictures();
}

void QPictureSlides::SetChangeInterval(int nInterval)
{
    m_nChangeInterval=nInterval;
    killTimer(m_nTimerId);
    m_nTimerId=startTimer(m_nChangeInterval);
}

void QPictureSlides::paintEvent(QPaintEvent*)
{
    QPainter painter;
    if(painter.begin(this))
    {
        int nWidgetW=width();
        int nWidgetH=height();
        if(!m_lstPictures.empty())
        {
            QObject* pCurrenObj=m_lstPictures.at(m_nCurrentPicIndex);
            if(pCurrenObj->metaObject()->className()==QStringLiteral("QPixmapObject"))                   //QPixmap
            {
                QPixmapObject* pPictureObj=qobject_cast<QPixmapObject*>(pCurrenObj);
                if(pPictureObj!=nullptr)
                {
                    QPixmap pixmap=pPictureObj->GetPixmap();
                    QSize drawPicSize=pixmap.size();
                    if(m_bIsScaledPicture &&
                            (drawPicSize.width()>nWidgetW || drawPicSize.height()>nWidgetH))
                    {
                        pixmap=pixmap.scaled(nWidgetW,nWidgetH,Qt::KeepAspectRatio);
                        drawPicSize=pixmap.size();
                    }
                    QTransform transForm;
                    transForm.rotate(-45,Qt::Axis::YAxis);
                    painter.setTransform(transForm);
                    int nStartX=qMax((nWidgetW-drawPicSize.width())/2,0);
                    int nStartY=qMax((nWidgetH-drawPicSize.height())/2,0);
                    painter.drawPixmap(nStartX,nStartY,pixmap);
                    painter.resetTransform();
                }
            }
            else if(pCurrenObj->metaObject()->className()==QStringLiteral("QMovie"))
            {
                QMovie* pMovie=qobject_cast<QMovie*>(pCurrenObj);
                if(pMovie!=nullptr)
                {
                    QPixmap currentPixmap=pMovie->currentPixmap();
                    if(!currentPixmap.isNull())
                    {
                        QSize picSize=currentPixmap.size();
                        if(m_bIsScaledPicture &&
                                (picSize.width()>nWidgetW || picSize.height()>nWidgetH))
                        {
                            currentPixmap=currentPixmap.scaled(nWidgetW,nWidgetH,Qt::KeepAspectRatio);
                            picSize=currentPixmap.size();
                        }
                        int nStartX=qMax((nWidgetW-picSize.width())/2,0);
                        int nStartY=qMax((nWidgetH-picSize.height())/2,0);
                        painter.drawPixmap(nStartX,nStartY,currentPixmap);
                    }
                }
            }
            else
            {
                painter.drawText(QRect(0,0,nWidgetW,nWidgetH),Qt::AlignCenter,QString("图片解析失败"));
            }
            if(m_nMouseEnterState!=0)                             //如果鼠标进入，绘制图片左右跳转的按钮
            {
                drawPictureChangeButton(painter);
            }
            drawProgressBar(painter);
        }
        else
        {
            painter.drawText(QRect(0,0,nWidgetW,nWidgetH),Qt::AlignCenter,QString("暂无图片"));
        }
        painter.end();
    }
    else
    {
        qWarning()<<"QPictureSlides::paintEvent error";
    }
}

void QPictureSlides::enterEvent(QEvent*)
{
    m_nMouseEnterState=1;
    update();
}

void QPictureSlides::leaveEvent(QEvent*)
{
    m_nMouseEnterState=0;
    update();
}

QRect QPictureSlides::GetButtonRect(int nType)
{
    int nWidgetHeight=height();
    int nWidgetWidth=width();
    int nButtonSize=qMin(static_cast<int>(nWidgetWidth*0.08),static_cast<int>(nWidgetHeight*0.08));
    int nRectStartY=(nWidgetHeight-nButtonSize)/2;
    if(nRectStartY<0 || nWidgetWidth<11 || nButtonSize<30)
    {
        return QRect(0,0,0,0);
    }
    int nRectStartX=0;
    if(nType==1)
    {
        nRectStartX=nWidgetWidth-nButtonSize-10;
    }
    else
    {
       nRectStartX=10;
    }
    return QRect(nRectStartX,nRectStartY,nButtonSize,nButtonSize);
}

void QPictureSlides::drawPictureChangeButton(QPainter& painter)
{
    QRect lRect=GetButtonRect(0);
    if(lRect.isValid())
    {
        painter.save();
        painter.translate(lRect.topLeft());
        QPainterPath painterPath;
        painterPath.arcMoveTo(0,0,lRect.height(),lRect.height(),90);
        QPointF pStartPos=painterPath.currentPosition();
        painterPath.arcTo(0,0,lRect.height(),lRect.height(),90,-180);
        QPointF pArcEndPos=painterPath.currentPosition();
        painterPath.lineTo(0,pArcEndPos.y());
        painterPath.lineTo(0,pStartPos.y());
        painterPath.closeSubpath();
        QPen emptyPen(Qt::NoPen);
        painter.setPen(emptyPen);
        QBrush backgroundBrush=m_nMouseEnterState==0x02?QBrush(QColor(140,140,140,200)):QBrush(QColor(120,120,120,150));
        painter.fillPath(painterPath,backgroundBrush);

        QBrush lineBrush=m_nMouseEnterState==0x02?QBrush(QColor(255,255,255)):QBrush(QColor(255,255,255,200));
        QPen drawPen(lineBrush,3,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin);
        painter.setPen(drawPen);
        QPainterPath patternPath;
        patternPath.moveTo(lRect.width()*3/5,lRect.height()/3);
        patternPath.lineTo(lRect.width()/4,lRect.height()/2);
        patternPath.lineTo(lRect.width()*3/5,lRect.height()*2/3);
        painter.drawPath(patternPath);
        painter.restore();
    }

    QRect rRect=GetButtonRect(1);
    if(rRect.isValid())
    {
        painter.save();
        painter.translate(rRect.topLeft());
        QPainterPath painterPath;
        painterPath.arcMoveTo(0,0,rRect.height(),rRect.height(),-90);
        QPointF pStartPos=painterPath.currentPosition();
        painterPath.arcTo(0,0,rRect.height(),rRect.height(),-90,-180);
        QPointF pArcEndPos=painterPath.currentPosition();
        painterPath.lineTo(rRect.width(),pArcEndPos.y());
        painterPath.lineTo(rRect.width(),pStartPos.y());
        painterPath.closeSubpath();
        QPen emptyPen(Qt::NoPen);
        painter.setPen(emptyPen);
        QBrush backgroundBrush=m_nMouseEnterState==0x03?QBrush(QColor(140,140,140,200)):QBrush(QColor(120,120,120,150));
        painter.fillPath(painterPath,backgroundBrush);

        QBrush lineBrush=m_nMouseEnterState==0x02?QBrush(QColor(255,255,255)):QBrush(QColor(255,255,255,200));
        QPen drawPen(lineBrush,3,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin);
        painter.setPen(drawPen);
        QPainterPath patternPath;
        patternPath.moveTo(rRect.width()*2/5,rRect.height()/3);
        patternPath.lineTo(rRect.width()*3/4,rRect.height()/2);
        patternPath.lineTo(rRect.width()*2/5,rRect.height()*2/3);
        painter.drawPath(patternPath);
        painter.restore();
    }
}

void QPictureSlides::drawProgressBar(QPainter& painter)
{
    int nWidgetHeight=height();
    int nWidgetWidth=width();
    if(nWidgetWidth<200 || nWidgetHeight<100)
    {
        return;
    }

    int nPointSize=static_cast<int>(qMin(nWidgetWidth*0.02,nWidgetHeight*0.02));
    int nPointSplit=nPointSize/3;

    if(m_lstPictures.size()<=5)
    {
        int nProgressWidth=m_lstPictures.size()*nPointSize+(m_lstPictures.size()-1)*nPointSplit;
        int nProgressStartX=(nWidgetWidth-nProgressWidth)/2;
        int nProgressStartY=(nWidgetHeight-nProgressWidth)*12/13;
        if(nProgressStartY<0)
        {
            return;
        }
        QPen emptyPen(Qt::NoPen);
        painter.setPen(emptyPen);
        for(int i=0;i<m_lstPictures.size();i++)
        {
            int nDrawPosX=nProgressStartX+(nPointSize+nPointSplit)*i;
            QBrush fillBrush=i==m_nCurrentPicIndex?QBrush(QColor(Qt::red)):QBrush(QColor(Qt::white));
            painter.setBrush(fillBrush);
            painter.drawEllipse(nDrawPosX,nProgressStartY,nPointSize,nPointSize);
        }
    }
    else
    {
        int nProgressWidth=nWidgetWidth/3;
        int nProgressHeight=nProgressWidth/20;
        int nProgressStartX=nWidgetWidth/3;
        int nProgressStartY=(nWidgetHeight-nProgressHeight)*12/13;
        if(nProgressStartY<0)
        {
            return;
        }
        QPen emptyPen(Qt::NoPen);
        painter.setPen(emptyPen);
        QBrush backgroundBrush(QColor(200,200,200));
        painter.setBrush(backgroundBrush);

        //painter.drawRoundRect(nProgressStartX,nProgressStartY,nProgressWidth,nProgressHeight,10,10);
        QPainterPath pathBackground=CreateRoundRectPainterPath(nProgressStartX,nProgressStartY,nProgressWidth,nProgressHeight);
        painter.drawPath(pathBackground);

        QBrush valueBrush(QColor(50,200,50));
        painter.setBrush(valueBrush);
        //painter.drawRoundRect(nProgressStartX+1,nProgressStartY+1,((nProgressWidth-2)*(m_nCurrentPicIndex+1)/m_lstPictures.size()),nProgressHeight-2,10,10);
        QPainterPath pathValue=CreateRoundRectPainterPath(nProgressStartX+1,nProgressStartY+1,((nProgressWidth-2)*(m_nCurrentPicIndex+1)/m_lstPictures.size()),nProgressHeight-2);
        painter.drawPath(pathValue);
    }
}

void QPictureSlides::timerEvent(QTimerEvent*)
{
    if(m_bIsAutoChange)
    {
        ChangeNextPicture();
    }
}

void QPictureSlides::mouseReleaseEvent(QMouseEvent* event)
{
    QRect lRect=GetButtonRect(0);
    if(lRect.contains(event->pos()))
    {
        ChangePrevPicture(true);
    }

    QRect rRect=GetButtonRect(1);
    if(rRect.contains(event->pos()))
    {
        ChangeNextPicture(true);
    }
}

void QPictureSlides::mouseMoveEvent(QMouseEvent* event)
{
    QRect lRect=GetButtonRect(0);
    if(lRect.contains(event->pos()))
    {
        m_nMouseEnterState=0x02;
        event->accept();
        update(lRect);
        return;
    }

    QRect rRect=GetButtonRect(1);
    if(rRect.contains(event->pos()))
    {
        m_nMouseEnterState=0x03;
        event->accept();
        update(rRect);
        return;
    }

    if(m_nMouseEnterState!=0x01)
    {
        update();
    }

    m_nMouseEnterState=0x01;
    event->accept();
    return;
}

QPainterPath QPictureSlides::CreateRoundRectPainterPath(int x,int y,int nWidth,int nHeight)
{
    QPainterPath path;
    path.arcMoveTo(x,y,nHeight,nHeight,-90);
    path.arcTo(x,y,nHeight,nHeight,-90,-180);
    QPointF p1=path.currentPosition();
    path.lineTo(x+nWidth-nHeight/2,p1.y());
    path.arcTo(x+nWidth-nHeight,p1.y(),nHeight,nHeight,90,-180);
    path.closeSubpath();
    return path;
}

void QPictureSlides::AddPictures(const QStringList& filePathList)
{
    foreach(QString str,filePathList)
    {
        if(str.endsWith(".gif"))
        {
            QMovie* pMovie=new QMovie(str);
            if(pMovie->isValid())
            {
                connect(pMovie,SIGNAL(updated(const QRect&)),this,SLOT(update()));
                m_lstPictures.push_back(qobject_cast<QObject*>(pMovie));
            }
            else
            {
                delete pMovie;
            }
        }
        else
        {
            QPixmap pixmap;
            if(pixmap.load(str))
            {
                QPixmapObject* pObject=new QPixmapObject(pixmap);
                m_lstPictures.push_back(qobject_cast<QObject*>(pObject));
            }
        }
    }

    if(!m_lstPictures.empty() && m_nCurrentPicIndex==0
            && (m_lstPictures.at(0)->metaObject()->className()==QStringLiteral("QMovie")))
    {
        QMovie* pMovie=qobject_cast<QMovie*>(m_lstPictures.at(0));
        if(pMovie->state()==QMovie::MovieState::NotRunning)
        {
            pMovie->start();
        }
    }
}

void QPictureSlides::ClearAllPictures()
{
    if(!m_lstPictures.empty())
    {
        for(QList<QObject*>::iterator it=m_lstPictures.begin();it!=m_lstPictures.end();it++)
        {
            delete (*it);
        }
        m_lstPictures.clear();
    }
}


void QPictureSlides::ChangeNextPicture(bool bKillTimer)
{
    if(m_lstPictures.at(m_nCurrentPicIndex)->metaObject()->className()==QStringLiteral("QMovie"))
    {
        (qobject_cast<QMovie*>(m_lstPictures.at(m_nCurrentPicIndex)))->stop();
    }

    m_nCurrentPicIndex=m_nCurrentPicIndex==(m_lstPictures.size()-1)?0:(m_nCurrentPicIndex+1);

    if(bKillTimer)
    {
        killTimer(m_nTimerId);
        m_nTimerId=startTimer(m_nChangeInterval);
    }

    if(m_lstPictures.at(m_nCurrentPicIndex)->metaObject()->className()==QStringLiteral("QMovie"))
    {
        (qobject_cast<QMovie*>(m_lstPictures.at(m_nCurrentPicIndex)))->start();
        return;
    }
    update();
}

void QPictureSlides::ChangePrevPicture(bool bKillTimer)
{
    if(m_lstPictures.at(m_nCurrentPicIndex)->metaObject()->className()==QStringLiteral("QMovie"))
    {
        (qobject_cast<QMovie*>(m_lstPictures.at(m_nCurrentPicIndex)))->stop();
    }
    m_nCurrentPicIndex=m_nCurrentPicIndex==0?(m_lstPictures.size()-1):(m_nCurrentPicIndex-1);
    if(bKillTimer)
    {
        killTimer(m_nTimerId);
        m_nTimerId=startTimer(m_nChangeInterval);
    }

    if(m_lstPictures.at(m_nCurrentPicIndex)->metaObject()->className()==QStringLiteral("QMovie"))
    {
        (qobject_cast<QMovie*>(m_lstPictures.at(m_nCurrentPicIndex)))->start();
        return;
    }
    update();
}
