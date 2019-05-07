#include "QFlowProgressTab.h"
#include <QPen>
#include <QDebug>
#include <QPainter>
#include <QMouseEvent>

static QPainterPath GetTabPainterPath(const QRect& rt,int nType)
{
    QPainterPath resultPath;
    resultPath.moveTo(rt.topLeft());
    if(nType==0)
    {
        resultPath.lineTo(rt.bottomLeft());
    }
    else
    {
        QPoint offsetPoint(static_cast<int>(rt.width()*0.1),rt.height()/2);
        QPoint dstPoint=rt.topLeft()+offsetPoint;
        resultPath.lineTo(dstPoint);
        resultPath.lineTo(rt.bottomLeft());
    }
    resultPath.lineTo(rt.bottomRight());
    if(nType==2)
    {
        resultPath.lineTo(rt.topRight());
    }
    else
    {
        QPoint offsetPoint(static_cast<int>(rt.width()*0.1),-rt.height()/2);
        QPoint dstPoint=rt.bottomRight()+offsetPoint;
        resultPath.lineTo(dstPoint);
        resultPath.lineTo(rt.topRight());
    }
    resultPath.lineTo(rt.topLeft());
    return resultPath;
}

QFlowProgressTab::QFlowProgressTab(QWidget* parent):QFlowProgressBase(parent)
{
    Init();
}

QFlowProgressTab::QFlowProgressTab(const QStringList& strMessageList,QWidget* parent):QFlowProgressBase(strMessageList,parent)
{
    Init();
}

void QFlowProgressTab::Init()
{
    SetBackgroundColor(QColor(230,230,230));
    m_clrNormalText.setRgb(0,0,0);
    m_clrFinishedText.setRgb(255,255,255);
    m_clrNormalCircle.setRgb(210,210,210);
    m_clrFinishedCircle.setRgb(255,255,255);
    m_clrNormalNumber.setRgb(255,255,255);
    m_clrFinishedNumber=GetFinishedBackgroundColor();
}


void QFlowProgressTab::DrawTabStyle(QPainter& painter)
{
    int nStepNums=GetStepNums();
    int nProgressLen=static_cast<int>(size().width()*0.9);
    int nProgressHeight=static_cast<int>(size().height()*0.6);
    int nCircleIconSize=static_cast<int>(nProgressHeight*0.4);
    int nIconStep=nProgressLen/nStepNums;
    int nIconColGap=static_cast<int>(nCircleIconSize*0.15);
    int nTextMaxLen=static_cast<int>(nIconStep*0.7-(nIconColGap+nCircleIconSize));

    if(nTextMaxLen<=0)
    {
        qWarning()<<"请调整控件宽高";
        return;
    }

    QBrush backgroundBrush(GetBackgroundColor());
    QBrush finishedBackgroundBrush(GetFinishedBackgroundColor());
    QPen emptyPen(Qt::NoPen);
    QPen finishedNumberPen(QBrush(m_clrFinishedNumber),4);
    QPen normalNumberPen(QBrush(m_clrNormalNumber),4);
    QPen normalTextPen(m_clrNormalText);
    QPen finishedTextPen(m_clrFinishedText);
    QBrush finishedCircleBrush(m_clrFinishedCircle);
    QBrush normalCircleBrush(m_clrNormalCircle);
    QPen spaceLinePen(QBrush(Qt::white),1);
    QFont ftNumber;
    QFont ftText;

    ftNumber.setPointSize(9);
    ftText.setPointSize(12);
    ftNumber.setBold(true);

    int nDrawStartX=static_cast<int>(size().width()*0.05);
    int nDrawStartY=static_cast<int>(size().height()*0.2);

    for(int i=0;i<nStepNums;i++)
    {
        int nOffsetPos=i*nIconStep;
        painter.setPen(emptyPen);
        if(i<GetCurrentStep())
        {
            painter.setBrush(finishedBackgroundBrush);
        }
        else
        {
            painter.setBrush(backgroundBrush);
        }
        QRect tabRect(nDrawStartX+nOffsetPos,nDrawStartY,nIconStep,nProgressHeight);
        int nRectType=i==0?0:(i==(nStepNums-1)?2:1);
        QPainterPath path=GetTabPainterPath(tabRect,nRectType);
        painter.drawPath(path);
        QString strShowText=GetAutoTextWidth()?SplitTextFixWidth(GetStepMessageList().at(i),ftText,nTextMaxLen):GetStepMessageList().at(i);
        QSize textSize=GetDrawTextSize(strShowText,ftText);
        int nIconStartOffset=(nIconStep-(nIconColGap+nCircleIconSize+textSize.width()))/2;
        nIconStartOffset=qMin(static_cast<int>(nIconStep*0.15),nIconStartOffset);
        painter.save();
        painter.translate(nDrawStartX+nOffsetPos,nDrawStartY);
        int nIconStartY=(nProgressHeight-nCircleIconSize)/2;
        if(i<GetCurrentStep())
        {
            painter.setBrush(finishedCircleBrush);
        }
        else
        {
            painter.setBrush(normalCircleBrush);
        }
        painter.drawEllipse(nIconStartOffset,nIconStartY,nCircleIconSize,nCircleIconSize);
        if(i<GetCurrentStep())
        {
            painter.setPen(finishedNumberPen);
        }
        else
        {
            painter.setPen(normalNumberPen);
        }
        painter.setFont(ftNumber);
        painter.drawText(QRect(nIconStartOffset,nIconStartY,nCircleIconSize,nCircleIconSize),Qt::AlignCenter,QString::number(i+1));
        if(i<GetCurrentStep())
        {
            painter.setPen(finishedTextPen);
        }
        else
        {
            painter.setPen(normalTextPen);
        }
        painter.setFont(ftText);
        int nTextStartOffset=nIconStartOffset+nCircleIconSize+nIconColGap;
        painter.drawText(QRect(nTextStartOffset,0,textSize.width(),nProgressHeight),Qt::AlignCenter,strShowText);
        painter.restore();
    }

    painter.setPen(spaceLinePen);

    for(int j=1;j<nStepNums;j++)
    {
        int nOffsetPos=j*nIconStep;
        int startX1=nDrawStartX+nOffsetPos;
        int startY1=nDrawStartY;
        int endX1=static_cast<int>(startX1+nIconStep*0.1);
        int endY1=startY1+nProgressHeight/2;
        int endX2=startX1;
        int endY2=startY1+nProgressHeight;
        painter.drawLine(startX1,startY1,endX1,endY1);
        painter.drawLine(endX1,endY1,endX2,endY2);
    }
}

void QFlowProgressTab::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    DrawTabStyle(painter);
}

void QFlowProgressTab::CalculeStepIconRect(QVector<QRect>& vec)
{
    int nStepNums=GetStepNums();
    int nProgressLen=static_cast<int>(size().width()*0.9);
    int nProgressHeight=static_cast<int>(size().height()*0.6);
    int nIconStep=nProgressLen/nStepNums;
    int nStartX=static_cast<int>(size().width()*0.05);
    int nStartY=static_cast<int>(size().height()*0.2);
    vec.clear();
    for(int i=0;i<nStepNums;i++)
    {
        QRect rt(nStartX+i*nIconStep,nStartY,nIconStep,nProgressHeight);
        vec.push_back(rt);
    }
}

void QFlowProgressTab::mouseReleaseEvent(QMouseEvent* e)
{
    QVector<QRect> vec;
    CalculeStepIconRect(vec);
    QPoint pt=e->pos();
    int iPos=0;
    foreach(QRect rt,vec)
    {
        if(rt.contains(pt,true))
        {
            ChangeCurrentStep(iPos+1);
            emit signals_stepClicked(iPos);
            if(GetOnStepClickListener()!=nullptr)
            {
                GetOnStepClickListener()(iPos);
                break;
            }
        }
        iPos++;
    }
}
