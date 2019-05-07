#include "QFlowProgressBar.h"
#include <QPainter>
#include <QFontMetrics>
#include <QVector>
#include <QMouseEvent>
#include <QDebug>

QFlowProgressBar::QFlowProgressBar(QWidget* parent):QFlowProgressBase(parent),
    m_barStyle(Styles::PROGRESS_BAR_CIRCLE_1)
{
    Init();
}

QFlowProgressBar::QFlowProgressBar(const QStringList& strDetailList,const Styles& style,QWidget* parent)
    :QFlowProgressBase(strDetailList,parent),m_barStyle(style)
{
    Init();
    CalculeStepIconRect(m_vecStepIconRect);
}

void QFlowProgressBar::Init()
{
    m_clrFinishNumber.setRgb(255,255,255);
}


void QFlowProgressBar::paintEvent(QPaintEvent* )
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    if(m_barStyle==Styles::PROGRESS_BAR_CIRCLE_1)
    {
        DrawCircle1Style(painter);
    }
    else if(m_barStyle==Styles::PROGRESS_BAR_CIRCLE_2)
    {
        DrawCircle2Style(painter);
    }
    else if(m_barStyle==Styles::PROGRESS_BAR_RECT)
    {
        DrawRectangleStyle(painter);
    }
    else
    {
        DrawCircle1Style(painter);
    }
}

void QFlowProgressBar::mouseReleaseEvent(QMouseEvent* e)
{
    QPoint pt=e->pos();
    int iPos=0;
    foreach(QRect rt,m_vecStepIconRect)
    {
        if(rt.contains(pt,true))
        {
            qDebug()<<"iPos clicked"<<iPos;
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

void QFlowProgressBar::DrawCircle1Style(QPainter& painter)
{
    int nStepNums=GetStepNums();
    int nProgressLen=static_cast<int>(size().width()*0.9);
    int nProgressHeight=static_cast<int>(size().height()*0.07);
    int nTotalStepIconLen=static_cast<int>(nProgressLen*0.88);
    int nIconSize=static_cast<int>((nTotalStepIconLen/(nStepNums-1))*0.15);
    //int nIconStep=(nTotalStepIconLen/(nStepNums-1))-nIconSize;
    int nIconStep=(nTotalStepIconLen/(nStepNums-1));
    int nTextGap=0;
    int nTextMaxLen=static_cast<int>(nIconStep*0.8);

    //画背景的进度条
    int nStartX=static_cast<int>(size().width()*0.05);
    int nStartY=static_cast<int>(size().height()*0.15);

    QBrush backgroundBrush(GetBackgroundColor());
    QBrush finishedBrush(GetFinishedBackgroundColor());
    QPen emptyPen(Qt::NoPen);
    painter.setPen(emptyPen);
    painter.setBrush(backgroundBrush);
    painter.drawRoundedRect(nStartX,nStartY,nProgressLen,nProgressHeight,nProgressHeight,nProgressHeight);

    //画出完成的进度
    if(GetCurrentStep()>0)
    {
        int nFinishedProgressLen=static_cast<int>(nProgressLen*0.05+(GetCurrentStep()-1)*nIconStep+nIconStep/2);
        nFinishedProgressLen=qMin(nFinishedProgressLen,nProgressLen);
        painter.setBrush(finishedBrush);
        painter.drawRoundedRect(nStartX,nStartY,nFinishedProgressLen,nProgressHeight,nProgressHeight,nProgressHeight);
    }

    int nIconStartY=nStartY-(qMax(nIconSize-nProgressHeight,0)/2);

    QPen iconBorderPen(QBrush(GetBackgroundColor()),nIconSize*0.2);
    QBrush whiteBrush(Qt::white);
    QFont ftNumber;
    QFont ftText;
    ftNumber.setPointSize(9);
    ftText.setPointSize(12);

    painter.save();

    painter.translate(nStartX+nProgressLen*0.05,nIconStartY);

    for(int i=0;i<nStepNums;i++)
    {
        if(i<GetCurrentStep())
        {
            painter.setBrush(finishedBrush);
        }
        else
        {
            painter.setBrush(whiteBrush);
        }
        int nCurrentXOffset=i*nIconStep;
        painter.setPen(iconBorderPen);
        painter.drawEllipse(nCurrentXOffset,0,nIconSize,nIconSize);
        painter.setFont(ftNumber);
        if(i<GetCurrentStep())
        {
            painter.setPen(QPen(m_clrFinishNumber));
        }
        painter.drawText(QRect(nCurrentXOffset,0,nIconSize,nIconSize),Qt::AlignCenter,QString::number(i+1));
        QString strShowText;
        if(!GetAutoTextWidth())
        {
            //textSize=GetDrawTextSize(m_strStepList.at(i),ftText);
            strShowText=GetStepMessageList().at(i);
        }
        else
        {
            strShowText=SplitTextFixWidth(GetStepMessageList().at(i),ftText,nTextMaxLen);
        }
        QSize textSize=GetDrawTextSize(strShowText,ftText);
        int nTextStartX=static_cast<int>((nCurrentXOffset+nIconSize*0.5)-textSize.width()/2);
        int nTextStartY=static_cast<int>(nIconSize*1.4+nTextGap);
        painter.setFont(ftText);
        if(i<GetCurrentStep())
        {
            painter.setPen(QPen(GetFinishedBackgroundColor()));
        }
        painter.drawText(QRect(nTextStartX,nTextStartY,textSize.width(),textSize.height()),Qt::AlignCenter,strShowText);
    }

    painter.restore();
}

void QFlowProgressBar::DrawCircle2Style(QPainter& painter)
{
    int nStepNums=GetStepNums();
    int nProgressLen=static_cast<int>(size().width()*0.9);
    int nProgressHeight=static_cast<int>(size().height()*0.07);
    int nTotalStepIconLen=static_cast<int>(nProgressLen*0.88);
    int nIconSize=static_cast<int>((nTotalStepIconLen/(nStepNums-1))*0.15);
    int nIconStep=(nTotalStepIconLen/(nStepNums-1));
    int nTextGap=static_cast<int>(size().height()*0.02);
    int nProgressGap=static_cast<int>(size().height()*0.02);
    int nTextMaxLen=static_cast<int>(nIconStep*0.8);

    //画背景的进度条
    int nStartX=static_cast<int>(size().width()*0.05);
    int nStartY=static_cast<int>(size().height()*0.15);

    int nProgressStartY=static_cast<int>(nStartY+nIconSize*1.3+nProgressGap);

    QBrush backgroundBrush(GetBackgroundColor());
    QBrush finishedBrush(GetFinishedBackgroundColor());
    QPen emptyPen(Qt::NoPen);
    painter.setPen(emptyPen);
    painter.setBrush(backgroundBrush);
    painter.drawRoundedRect(nStartX,nProgressStartY,nProgressLen,nProgressHeight,nProgressHeight,nProgressHeight);

    //画出完成的进度
    if(GetCurrentStep()>0)
    {
        int nFinishedProgressLen=static_cast<int>(nProgressLen*0.05+(GetCurrentStep()-1)*nIconStep+nIconStep/2);
        nFinishedProgressLen=qMin(nFinishedProgressLen,nProgressLen);
        painter.setBrush(finishedBrush);
        painter.drawRoundedRect(nStartX,nProgressStartY,nFinishedProgressLen,nProgressHeight,nProgressHeight,nProgressHeight);
    }

    QPen iconBorderPen(QBrush(GetBackgroundColor()),nIconSize*0.2);
    QBrush whiteBrush(Qt::white);
    QFont ftNumber;
    QFont ftText;
    ftNumber.setPointSize(9);
    ftText.setPointSize(12);


    painter.save();
    painter.translate(nStartX+nProgressLen*0.05,nStartY);

    for(int i=0;i<GetStepNums();i++)
    {
        if(i<GetCurrentStep())
        {
            painter.setBrush(finishedBrush);
        }
        else
        {
            painter.setBrush(whiteBrush);
        }
        int nCurrentXOffset=i*nIconStep;
        painter.setPen(iconBorderPen);
        painter.drawEllipse(nCurrentXOffset,0,nIconSize,nIconSize);

        if(i<GetCurrentStep())
        {
            painter.setBrush(GetFinishedBackgroundColor());
        }
        else
        {
            painter.setBrush(GetBackgroundColor());
        }
        painter.setPen(emptyPen);
        QPainterPath painterFillPath;
        painterFillPath.moveTo(nCurrentXOffset+nIconSize/2,nIconSize*1.15);
        painterFillPath.lineTo(nCurrentXOffset+nIconSize*0.3,nIconSize*1.3+nProgressGap);
        painterFillPath.lineTo(nCurrentXOffset+nIconSize*0.7,nIconSize*1.3+nProgressGap);
        painterFillPath.closeSubpath();
        painter.drawPath(painterFillPath);

        painter.setFont(ftNumber);
        if(i<GetCurrentStep())
        {
            painter.setPen(QPen(m_clrFinishNumber));
        }
        else
        {
            painter.setPen(QPen(GetBackgroundColor()));
        }
        painter.drawText(QRect(nCurrentXOffset,0,nIconSize,nIconSize),Qt::AlignCenter,QString::number(i+1));

        QString strShowText;
        if(!GetAutoTextWidth())
        {
            //textSize=GetDrawTextSize(m_strStepList.at(i),ftText);
            strShowText=GetStepMessageList().at(i);
        }
        else
        {
            strShowText=SplitTextFixWidth(GetStepMessageList().at(i),ftText,nTextMaxLen);
        }
        QSize textSize=GetDrawTextSize(strShowText,ftText);
        int nTextStartX=static_cast<int>((nCurrentXOffset+nIconSize*0.5)-textSize.width()/2);
        int nTextStartY=static_cast<int>(nIconSize*1.3+nTextGap+nProgressGap+nProgressHeight);
        painter.setFont(ftText);
        if(i<GetCurrentStep())
        {
            painter.setPen(QPen(GetFinishedBackgroundColor()));
        }
        painter.drawText(QRect(nTextStartX,nTextStartY,textSize.width(),textSize.height()),Qt::AlignCenter,strShowText);
    }
    painter.restore();
}

void QFlowProgressBar::DrawRectangleStyle(QPainter& painter)
{
    int nStepNums=GetStepNums();
    int nProgressLen=static_cast<int>(size().width()*0.9);
    int nProgressWeight=static_cast<int>(size().height()*0.005);
    int nIconSize=static_cast<int>((nProgressLen/(nStepNums-1))*0.15);
    int nIconStep=nProgressLen/(nStepNums-1);
    int nTextGap=static_cast<int>(size().height()*0.04);
    int nTextMaxLen=static_cast<int>(nIconStep*0.8);
    const int SPACE_GAP=4;

    QBrush backgroundBrush(GetBackgroundColor());
    QPen backgroundPen(GetBackgroundColor());
    QPen emptyPen(Qt::NoPen);
    QBrush whiteBrush(Qt::white);
    QPen finishedNumberPen(QBrush(m_clrFinishNumber),2);
    QBrush finishedBrush(GetFinishedBackgroundColor());
    QPen finishedPen(QBrush(GetFinishedBackgroundColor()),2);
    QFont ftNumber;
    QFont ftText;
    QPen backgroundLinePen(backgroundBrush,nProgressWeight);
    QPen finishedLinePen(finishedBrush,nProgressWeight);

    ftNumber.setPointSize(9);
    ftText.setPointSize(12);

    int nDrawStartX=static_cast<int>(size().width()*0.05);
    int nDrawStartY=static_cast<int>(size().height()*0.15);

    painter.save();
    painter.translate(nDrawStartX,nDrawStartY);

    for(int i=0;i<nStepNums;i++)
    {
        int nCurrentXOffset=i*nIconStep;
        if(i<GetCurrentStep())
        {
            painter.setBrush(finishedBrush);
            painter.setPen(finishedPen);
        }
        else
        {
            painter.setBrush(whiteBrush);
            painter.setPen(backgroundPen);
        }
        painter.drawRect(nCurrentXOffset,0,nIconSize,nIconSize);
        if(i<GetCurrentStep())
        {
            painter.setPen(finishedNumberPen);
        }
        else
        {
            painter.setPen(backgroundPen);
        }
        painter.setFont(ftNumber);
        painter.drawText(QRect(nCurrentXOffset,0,nIconSize,nIconSize),Qt::AlignCenter,QString::number(i+1));
        if(i<GetCurrentStep())
        {
            painter.setPen(finishedPen);
        }
        QString strShowText;
        if(!GetAutoTextWidth())
        {
            strShowText=GetStepMessageList().at(i);
        }
        else
        {
            strShowText=SplitTextFixWidth(GetStepMessageList().at(i),ftText,nTextMaxLen);
        }
        QSize textSize=GetDrawTextSize(strShowText,ftText);
        int nTextStartX=static_cast<int>((nCurrentXOffset+nIconSize*0.5)-textSize.width()/2);
        int nTextStartY=static_cast<int>(nIconSize+nTextGap);
        painter.setFont(ftText);
        painter.drawText(QRect(nTextStartX,nTextStartY,textSize.width(),textSize.height()),Qt::AlignCenter,strShowText);
        painter.setPen(emptyPen);
        if(i<GetCurrentStep())
        {
            painter.setPen(finishedLinePen);
        }
        else
        {
            painter.setPen(backgroundLinePen);
        }
        if(i==0)
        {
            int nLineStartX=nCurrentXOffset+nIconSize;
            int nLineStartY=(nIconSize-nProgressWeight)/2;
            int nLineEndX=nLineStartX+(nIconStep-nIconSize-SPACE_GAP)/2;
            int nLineEndY=nLineStartY;
            painter.drawLine(nLineStartX,nLineStartY,nLineEndX,nLineEndY);
        }
        else if(i==nStepNums-1)
        {
            int nLineStartX=nCurrentXOffset;
            int nLineStartY=(nIconSize-nProgressWeight)/2;
            int nLineEndX=nLineStartX-(nIconStep-nIconSize-SPACE_GAP)/2;
            int nLineEndY=nLineStartY;
            painter.drawLine(nLineStartX,nLineStartY,nLineEndX,nLineEndY);
        }
        else
        {
            int nLineStartX1=nCurrentXOffset+nIconSize;
            int nLineStartY1=(nIconSize-nProgressWeight)/2;
            int nLineEndX1=nLineStartX1+(nIconStep-nIconSize-SPACE_GAP)/2;
            int nLineEndY1=nLineStartY1;
            int nLineStartX2=nCurrentXOffset;
            int nLineStartY2=(nIconSize-nProgressWeight)/2;
            int nLineEndX2=nLineStartX2-(nIconStep-nIconSize-SPACE_GAP)/2;
            int nLineEndY2=nLineStartY2;
            QLine lineArray[2]={QLine(nLineStartX1,nLineStartY1,nLineEndX1,nLineEndY1),
                               QLine(nLineStartX2,nLineStartY2,nLineEndX2,nLineEndY2)};
            painter.drawLines(lineArray,2);
        }
    }

    painter.restore();
}

void QFlowProgressBar::CalculeStepIconRect(QVector<QRect>& vec)
{
    int nStepNums=GetStepNums();
    int nProgressLen=static_cast<int>(size().width()*0.9);
    int nIconSize=static_cast<int>((nProgressLen/(nStepNums-1))*0.2);
    int nIconStep=m_barStyle==Styles::PROGRESS_BAR_RECT?nProgressLen/(nStepNums-1):static_cast<int>((nProgressLen*0.9)/(nStepNums-1));

    int nStartX=m_barStyle==Styles::PROGRESS_BAR_RECT?static_cast<int>(size().width()*0.05):static_cast<int>(size().width()*0.07);
    int nStartY=static_cast<int>(size().height()*0.15);

    vec.clear();
    for(int i=0;i<nStepNums;i++)
    {
        int nXOffset=i*nIconStep;
        QRect rt(nStartX+nXOffset,nStartY,nIconSize,nIconSize);
        vec.push_back(rt);
    }
}

void QFlowProgressBar::SetStepMessageList(const QStringList& list)
{
    QFlowProgressBase::SetStepMessageList(list);
    CalculeStepIconRect(m_vecStepIconRect);
}

