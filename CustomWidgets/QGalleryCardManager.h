#ifndef QGALLERYCARDMANAGER_H
#define QGALLERYCARDMANAGER_H
#include <QList>
#include <QObject>
#include "QGalleryCard.h"

//typedef void (*transform_callback)(int,int);                                //0:正在执行 1:完成 -1:错误

class QGalleryTransfer
{
public:
    QGalleryTransfer()
    {
        m_nCurrentStep=m_nTotalStep=0;
        m_nMoveStep=m_nRoateStep=m_nRoateMoveStep=m_dSizeHeightStep=m_dSizeWidthStep=0.0;
    }
    ~QGalleryTransfer()
    {

    }
    void Reset(int nTotalStep=0,double dMoveStep=0,double dRoateStep=0,double dRoateMoveStep=0
            ,double dSizeWidthStep=0,double dSizeHeightStep=0)
    {
        m_nCurrentStep=0;
        m_nTotalStep=nTotalStep;
        m_nMoveStep=dMoveStep;
        m_nRoateStep=dRoateStep;
        m_nRoateMoveStep=dRoateMoveStep;
        m_dSizeWidthStep=dSizeWidthStep;
        m_dSizeHeightStep=dSizeHeightStep;
    }
    void AddStep(){m_nCurrentStep++;}
    int GetCurrentStep(){return m_nCurrentStep;}
    bool IsFinished(){return m_nCurrentStep>=m_nTotalStep;}
    void SetMoveStep(double nMoveStep){m_nMoveStep=nMoveStep;}
    double GetMoveStep(){return m_nMoveStep;}
    void SetRoateMoveStep(double nStep){m_nRoateMoveStep=nStep;}
    double GetRoateMoveStep(){return m_nRoateMoveStep;}
    void SetRoateStep(double nStep){m_nRoateStep=nStep;}
    double GetRoateStep(){return m_nRoateStep;}
    double GetSizeWidthStep(){return m_dSizeWidthStep;}
    double GetSizeHeightStep(){return m_dSizeHeightStep;}

private:
    int m_nCurrentStep;
    int m_nTotalStep;
    double m_nMoveStep;
    double m_nRoateMoveStep;
    double m_nRoateStep;
    double m_dSizeWidthStep;
    double m_dSizeHeightStep;
};

class QGalleryCardManager:public QObject
{
    Q_OBJECT
public:
    enum TRANSFER_SPEED{NORMAL,FAST};
    QGalleryCardManager(QObject* parent=nullptr);
    ~QGalleryCardManager() override;
    void ChangeGalleryCard(const QList<QPixmap>& showList,int nShowIndex,const QSize& windowSize);
    void StartTransform(int nType);                  //开始转场动画
    //void SetTransformListener(transform_callback l){m_pTransformCallback=l;}
    void ClearCache(){m_lstCurrentShowCard.clear();}
    const QList<QGalleryCard>& GetGalleryCardList(){return m_lstCurrentShowCard;}
signals:
    void signals_transformPlay(int,int);

protected:
    virtual void AdjustGalleryPosition(QList<QGalleryCard>& lstCard,int nShowIndex,const QSize& windowSize);
    virtual void timerEvent(QTimerEvent *event) override;
private:
    QList<QGalleryCard> m_lstCurrentShowCard;
    QGalleryTransfer m_galleryTransfer;
    //transform_callback m_pTransformCallback;                            //动画转换状态回调
    int m_nTransferType;
    TRANSFER_SPEED m_nSpeed;
    bool m_bIsTransferRunning;
    int m_nTimerId;
    int m_nCardSpan;
    int m_nTabCardSpan;
    int m_nShowIndex;
    int m_nScaleWidth;
    int m_nScaleHeight;

    QList<QSize> GetGalleryCardSize(int nShowIndex,int nTotalNums,const QSize& windowSize);
};

#endif // QGALLERYCARDMANAGER_H
