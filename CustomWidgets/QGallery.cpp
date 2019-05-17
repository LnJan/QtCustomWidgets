#include "QGallery.h"
#include <QPainter>

QGallery::QGallery(QWidget *parent) : QWidget(parent)
{
    m_clrBackground.setRgb(30,30,30);
    m_nCurrentCardIndex=0;
    m_nShowCardNums=11;
    connect(&m_galleryManager,SIGNAL(signals_transformPlay(int,int)),this,SLOT(slots_transformPlay(int,int)));
}

QGallery::~QGallery()
{
    disconnect(&m_galleryManager,SIGNAL(signals_transformPlay(int,int)),this,SLOT(slots_transformPlay(int,int)));
}

bool QGallery::InitImageList(const QStringList& strImagePaths)
{
    m_lstImages.clear();
    foreach(QString str,strImagePaths)
    {
        QPixmap readPixmap;
        if(readPixmap.load(str))
        {
            m_lstImages.push_back(readPixmap);
        }
    }
    if(m_lstImages.empty())
    {
        m_galleryManager.ClearCache();
    }
    else
    {
        QList<QPixmap> showList;
        int nShowIndex=0;
        GetCurrentGalleryCards(showList,nShowIndex);
        m_galleryManager.ChangeGalleryCard(showList,nShowIndex,size());
    }
    update();
    return true;
}

void QGallery::ClearAllImages()
{
    m_lstImages.clear();
    m_galleryManager.ClearCache();
    update();
}

void QGallery::GetCurrentGalleryCards(QList<QPixmap>& resultList,int& nIndex)
{
    resultList.clear();
    if(m_lstImages.empty())
    {
        return;
    }
    int nHalfCardNums=m_nShowCardNums/2;
    int nLeftIndex=qMax(m_nCurrentCardIndex-nHalfCardNums,0);
    int nRightIndex=m_nCurrentCardIndex+nHalfCardNums;
    nIndex=m_nCurrentCardIndex-nLeftIndex;
    for(int i=nLeftIndex;i<qMin(m_lstImages.size(),nRightIndex+1);i++)
    {
        resultList.push_back(m_lstImages.at(i));
    }

    if(nRightIndex>m_lstImages.size()-1)
    {
        //int nAddEmptyCardNum=nHalfCardNums-(nRightIndex-nIndex);
        int nAddEmptyCardNum=nRightIndex-m_lstImages.size()+1;
        while(nAddEmptyCardNum>0)
        {
            QPixmap emptyPixmap;
            resultList.push_back(emptyPixmap);
            nAddEmptyCardNum--;
        }
    }
}

bool QGallery::InsertImage(const QString& strImagePath)
{
    QPixmap pixmap;
    if(pixmap.load(strImagePath))
    {
        m_lstImages.push_back(pixmap);
    }
    else
    {
        return  false;
    }

    QList<QPixmap> showList;
    int nShowIndex=0;
    GetCurrentGalleryCards(showList,nShowIndex);
    m_galleryManager.ChangeGalleryCard(showList,nShowIndex,size());
    update();
    return true;
}

void QGallery::SetShowCardNums(int nNums)
{

    m_nShowCardNums=nNums;

    QList<QPixmap> showList;
    int nShowIndex=0;
    GetCurrentGalleryCards(showList,nShowIndex);
    m_galleryManager.ChangeGalleryCard(showList,nShowIndex,size());
    update();
}

void QGallery::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);

    painter.fillRect(QRect(0,0,size().width(),size().height()),QBrush(m_clrBackground));

    QList<QGalleryCard> cardList=m_galleryManager.GetGalleryCardList();

    if(!cardList.empty())
    {
        int nShowIndex=0;
        for(int i=0;i<cardList.size();i++)
        {
            if(static_cast<int>(cardList[i].GetRoateAngle())==0)
            {
                nShowIndex=i;
                break;
            }
            //cardList[i].DrawCard(painter);
        }

        for(int i=0;i<nShowIndex;i++)
        {
            cardList[i].DrawCard(painter);
        }

        for(int j=cardList.size()-1;j>nShowIndex;j--)
        {
            cardList[j].DrawCard(painter);
        }
        cardList[nShowIndex].DrawCard(painter);
    }

}

void QGallery::slots_transformPlay(int nType,int nStatus)
{
    if(nStatus==1)
    {
        if(nType==0)
        {
            m_nCurrentCardIndex++;
        }
        else
        {
            m_nCurrentCardIndex--;
        }
        QList<QPixmap> showList;
        int nShowIndex=0;
        GetCurrentGalleryCards(showList,nShowIndex);
        m_galleryManager.ChangeGalleryCard(showList,nShowIndex,size());
    }
    update();
}

void QGallery::MoveStep(int nType)
{
    if(nType==1)
    {
        if(m_nCurrentCardIndex<1)
        {
            return;
        }
    }

    m_galleryManager.StartTransform(nType);
}




