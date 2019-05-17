#ifndef QGALLERY_H
#define QGALLERY_H

#include <QWidget>
#include <QList>
#include "QGalleryCardManager.h"

class QGallery : public QWidget
{
    Q_OBJECT
public:
    explicit QGallery(QWidget *parent = nullptr);
    virtual ~QGallery() override;
    void SetBackgroundColor(const QColor& color){m_clrBackground=color;}
    const QColor& GetBackgroundColor() const{return m_clrBackground;}
    bool InitImageList(const QStringList& strImagePaths);
    bool InsertImage(const QString& strImagePath);
    void ClearAllImages();
    int GetCurrentCardIndex(){return m_nCurrentCardIndex;}
    void SetShowCardNums(int nNums);
    int GetShowCardNums(){return m_nShowCardNums;}
    void MoveStep(int nType);

protected:
    virtual void paintEvent(QPaintEvent *event) override;
    //virtual void resizeEvent(QResizeEvent *event) override;
    //计算卡片的大小和位置
signals:

public slots:
    void slots_transformPlay(int nType,int nStatus);
private:
    QColor m_clrBackground;
    QList<QPixmap> m_lstImages;                             //保存所有图片
    QGalleryCardManager m_galleryManager;
    int m_nCurrentCardIndex;
    int m_nShowCardNums;

    void GetCurrentGalleryCards(QList<QPixmap>& resultList,int& nIndex);
};

#endif // QGALLERY_H
