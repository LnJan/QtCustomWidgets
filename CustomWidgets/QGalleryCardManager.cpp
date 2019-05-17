#include "QGalleryCardManager.h"
#include <QDebug>

static const double CARD_ANGLE=65.0;
static const int CARD_OPACITY=200;
static const int FAST_STEPS=30;
static const int NORMAL_STEPS=50;
static const int INTERVAL_TIMER=1200/NORMAL_STEPS;

QGalleryCardManager::QGalleryCardManager(QObject* parent):QObject(parent)
  ,m_nTransferType(0),m_nSpeed(TRANSFER_SPEED::NORMAL),m_bIsTransferRunning(false),m_nTimerId(-1)
  ,m_nCardSpan(0),m_nTabCardSpan(0),m_nShowIndex(0),m_nScaleWidth(0),m_nScaleHeight(0)
{

}

QGalleryCardManager::~QGalleryCardManager()
{

}

void QGalleryCardManager::ChangeGalleryCard(const QList<QPixmap>& showList,int nShowIndex,const QSize& windowSize)
{
    QList<QSize> cardSizeList=GetGalleryCardSize(nShowIndex,showList.size(),windowSize);
    m_lstCurrentShowCard.clear();
    for(int i=0;i<showList.size();i++)
    {
        QGalleryCard tmpCard(0,0,cardSizeList.at(i).width(),cardSizeList.at(i).height(),showList.at(i));
        m_lstCurrentShowCard.push_back(tmpCard);
    }
    AdjustGalleryPosition(m_lstCurrentShowCard,nShowIndex,windowSize);
}

QList<QSize> QGalleryCardManager::GetGalleryCardSize(int nShowIndex,int nTotalNums,const QSize& windowSize)
{
    int nHeightTmp=windowSize.height()*2/3;
    int nCardHeight=nHeightTmp*4/5;
    int nCardWidth=static_cast<int>(windowSize.width()/5.5);

    int nCardWidthSmall=static_cast<int>(nCardWidth*0.95);
    int nCardHeightSmall=static_cast<int>(nCardHeight*0.95);
    int nWidthScale=static_cast<int>(nCardWidthSmall*0.01);
    int nHeightScale=static_cast<int>(nCardHeightSmall*0.01);
    int nCardWidthMin=static_cast<int>(nCardWidth*0.5);
    int nCardHeightMin=static_cast<int>(nCardHeight*0.5);

    m_nScaleWidth=nCardWidth-nCardWidthSmall;
    m_nScaleHeight=nCardHeight-nCardHeightSmall;

    QList<QSize> resultList;
    for(int i=0;i<nTotalNums;i++)
    {
        QSize resultSize;
        if(i==nShowIndex)
        {
            resultSize.setWidth(nCardWidth);
            resultSize.setHeight(nCardHeight);
        }
        else if(i<nShowIndex)
        {
            int nTmp=nShowIndex-i-1;
            resultSize.setWidth(qMax(nCardWidthSmall-nTmp*nWidthScale,nCardWidthMin));
            resultSize.setHeight(qMax(nCardHeightSmall-nTmp*nHeightScale,nCardHeightMin));
        }
        else
        {
            int nTmp=i-nShowIndex-1;
            resultSize.setWidth(qMax(nCardWidthSmall-nTmp*nWidthScale,nCardWidthMin));
            resultSize.setHeight(qMax(nCardHeightSmall-nTmp*nHeightScale,nCardHeightMin));
        }
        resultList.push_back(resultSize);
    }
    return resultList;
}

void QGalleryCardManager::AdjustGalleryPosition(QList<QGalleryCard>& lstCard,int nShowIndex,const QSize& windowSize)
{
    int nCardNums=lstCard.size();
    if(nCardNums>0)
    {
        int nLeftCards=nShowIndex;
        int nRightCards=nCardNums-nShowIndex-1;
        int nEndsSpace=static_cast<int>(windowSize.width()*0.25);
        int nLeftCardSpan=nLeftCards>1?nEndsSpace/(nLeftCards-1):nEndsSpace/4;
        int nRightCardSpan=nRightCards>1?nEndsSpace/(nRightCards-1):nEndsSpace/4;
        //nLeftCardSpan=qMin(nLeftCardSpan,nEndsSpace/4);
        //nRightCardSpan=qMin(nRightCardSpan,nEndsSpace/4);
        int nCardSpan=qMin(nLeftCardSpan,nRightCardSpan);
        int nMaxCardWidth=lstCard[nShowIndex].GetCardRegion().width();
        m_nCardSpan=nCardSpan;
        m_nShowIndex=nShowIndex;
        for(int i=0;i<nCardNums;i++)
        {
            int nCardStartY=(windowSize.height()*2/3)-lstCard[i].GetCardRegion().height();
            if(i<nShowIndex)
            {
                if(i==0)
                {
                    lstCard[i].SetOpacity(CARD_OPACITY);
                }
                lstCard[i].SetRoateAngle(0-CARD_ANGLE);
                lstCard[i].MoveTo(QPoint(windowSize.width()*0.05+nEndsSpace-(nShowIndex-i-1)*nCardSpan,nCardStartY));
            }
            else if(i==nShowIndex)
            {
                int nCardStartX=(windowSize.width()-lstCard[i].GetCardRegion().width())/2;
                lstCard[i].MoveTo(QPoint(nCardStartX,nCardStartY));
            }
            else
            {
                if(i==nCardNums-1)
                {
                    lstCard[i].SetOpacity(CARD_OPACITY);
                }
                lstCard[i].SetRoateAngle(CARD_ANGLE);
                int nTmpX=static_cast<int>(windowSize.width()*0.95-nEndsSpace-nMaxCardWidth*(CARD_ANGLE/180));
                lstCard[i].MoveTo(QPoint(nTmpX+(i-nShowIndex-1)*nCardSpan,nCardStartY));
            }
        }
        m_nTabCardSpan=lstCard[nShowIndex].GetCardRegion().left()-static_cast<int>(windowSize.width()*0.3);
    }
}

void QGalleryCardManager::timerEvent(QTimerEvent *event)
{
    m_galleryTransfer.AddStep();
    for(int i=0;i<m_lstCurrentShowCard.size();i++)
    {
        QPointF pt=m_lstCurrentShowCard[i].GetCardRegion().topLeft();
        if(m_nTransferType==0)
        {
            if(i==m_nShowIndex+1)
            {
                pt.setX(pt.x()+m_galleryTransfer.GetRoateMoveStep()*2);
                pt.setY(pt.y()-m_galleryTransfer.GetSizeHeightStep());
                QRectF rectF=m_lstCurrentShowCard[i].GetCardRegion();
                m_lstCurrentShowCard[i].MoveTo(pt);
                m_lstCurrentShowCard[i].SetCardSize(rectF.width()+m_galleryTransfer.GetSizeWidthStep(),rectF.height()+m_galleryTransfer.GetSizeHeightStep());
                m_lstCurrentShowCard[i].SetRoateAngle(CARD_ANGLE-m_galleryTransfer.GetCurrentStep()*m_galleryTransfer.GetRoateStep());
            }
            else if(i==m_nShowIndex)
            {
                pt.setX(pt.x()+m_galleryTransfer.GetRoateMoveStep());
                pt.setY(pt.y()+m_galleryTransfer.GetSizeHeightStep());
                QRectF rectF=m_lstCurrentShowCard[i].GetCardRegion();
                m_lstCurrentShowCard[i].MoveTo(pt);
                m_lstCurrentShowCard[i].SetCardSize(rectF.width()-m_galleryTransfer.GetSizeWidthStep(),rectF.height()-m_galleryTransfer.GetSizeHeightStep());
                m_lstCurrentShowCard[i].SetRoateAngle(0-m_galleryTransfer.GetCurrentStep()*m_galleryTransfer.GetRoateStep());
            }
            else
            {
                pt.setX(pt.x()+m_galleryTransfer.GetMoveStep());
                m_lstCurrentShowCard[i].MoveTo(pt);
            }
        }
        else
        {
            if(i==m_nShowIndex)
            {
                pt.setX(pt.x()+m_galleryTransfer.GetRoateMoveStep()*2);
                pt.setY(pt.y()+m_galleryTransfer.GetSizeHeightStep());
                QRectF rectF=m_lstCurrentShowCard[i].GetCardRegion();
                m_lstCurrentShowCard[i].MoveTo(pt);
                m_lstCurrentShowCard[i].SetRoateAngle(m_galleryTransfer.GetCurrentStep()*m_galleryTransfer.GetRoateStep());
                m_lstCurrentShowCard[i].SetCardSize(rectF.width()-m_galleryTransfer.GetSizeWidthStep(),rectF.height()-m_galleryTransfer.GetSizeHeightStep());
            }
            else if(i==m_nShowIndex-1)
            {
                pt.setX(pt.x()+m_galleryTransfer.GetRoateMoveStep());
                pt.setY(pt.y()-m_galleryTransfer.GetSizeHeightStep());
                QRectF rectF=m_lstCurrentShowCard[i].GetCardRegion();
                m_lstCurrentShowCard[i].MoveTo(pt);
                m_lstCurrentShowCard[i].SetRoateAngle(0-CARD_ANGLE+m_galleryTransfer.GetCurrentStep()*m_galleryTransfer.GetRoateStep());
                m_lstCurrentShowCard[i].SetCardSize(rectF.width()+m_galleryTransfer.GetSizeWidthStep(),rectF.height()+m_galleryTransfer.GetSizeHeightStep());
            }
            else
            {
                pt.setX(pt.x()+m_galleryTransfer.GetMoveStep());
                m_lstCurrentShowCard[i].MoveTo(pt);
            }
        }
    }
    if(m_galleryTransfer.IsFinished())
    {
        emit signals_transformPlay(m_nTransferType,1);
        killTimer(event->timerId());
        m_bIsTransferRunning=false;
    }
    else
    {
        emit signals_transformPlay(m_nTransferType,0);
    }
}

void QGalleryCardManager::StartTransform(int nType)
{
    if(nType!=0 && nType!=1)
    {
        return;
    }
    if(!m_bIsTransferRunning)
    {
        m_bIsTransferRunning=true;
        m_nTransferType=nType;
        int nTotalStep=m_nSpeed==TRANSFER_SPEED::NORMAL?NORMAL_STEPS:FAST_STEPS;
        double dRoate=CARD_ANGLE/nTotalStep;
        double dMoveStep=(double)(m_nCardSpan)/nTotalStep;
        double dRoateMoveStep=(double)(m_nTabCardSpan)/nTotalStep;
        double dScaleWidth=(double)(m_nScaleWidth)/nTotalStep;
        double dScaleHeight=(double)(m_nScaleHeight)/nTotalStep;
        if(nType==0)
        {
            dMoveStep=0-dMoveStep;
            dRoateMoveStep=0-dRoateMoveStep;
        }
        m_galleryTransfer.Reset(nTotalStep,dMoveStep,dRoate,dRoateMoveStep,dScaleWidth,dScaleHeight);
        m_nTimerId=startTimer(INTERVAL_TIMER);
    }
}
