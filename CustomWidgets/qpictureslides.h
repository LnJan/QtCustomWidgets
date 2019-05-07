/*********************************************************************************
  * Copyright(C) 2019,保留所有权利。( All rights reserved. )
  * FileName:qpictureslides.h
  * Author:Ln_Jan
  * Version:v1.0.0
  * Date:2019-04-16
  * Description:导航页图片浏览器
  * Others:
  * Function List:
  * History:
  *
**********************************************************************************/
#ifndef QPICTURESLIDES_H
#define QPICTURESLIDES_H

#include <QLabel>
#include <QList>

class QPictureSlides : public QLabel
{
    Q_OBJECT
public:
    explicit QPictureSlides(QWidget *parent = nullptr);
    virtual ~QPictureSlides() override;
    int GetPictureNums(){return m_lstPictures.size();}
    void AddPictures(const QStringList& filePathList);
    void ClearAllPictures();
    void SetAutoChange(bool bChange){m_bIsAutoChange=bChange;}
    bool IsAutoChange(){return m_bIsAutoChange;}
    void SetScaledPicture(bool bScaled){m_bIsScaledPicture=bScaled;}
    bool IsScaledPicture(){return m_bIsScaledPicture;}
    void SetChangeInterval(int nInterval);
    int GetChangeInterval(){return m_nChangeInterval;}

protected:
    void paintEvent(QPaintEvent *paintEvent) override;
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void timerEvent(QTimerEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

    virtual void drawPictureChangeButton(QPainter& painter);
    virtual void drawProgressBar(QPainter& painter);
signals:

private slots:

private:
    QList<QObject*> m_lstPictures;
    bool m_bIsAutoChange;
    bool m_bIsScaledPicture;
    int m_nChangeInterval;
    int m_nMouseEnterState;                     //0:鼠标没进入控件 1:鼠标进入控件 2:鼠标在左按钮 3:鼠标在右按钮
    int m_nCurrentPicIndex;
    int m_nTimerId;

    QRect GetButtonRect(int nType);             //0:left 1:right
    QPainterPath CreateRoundRectPainterPath(int x,int y,int nWidth,int nHeight);

    void ChangeNextPicture(bool bKillTimer=false);
    void ChangePrevPicture(bool bKillTimer=false);
};

#endif // QPICTURESLIDES_H
