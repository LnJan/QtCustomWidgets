/*********************************************************************************
  * Copyright(C) 2019,保留所有权利。( All rights reserved. )
  * FileName:QFlowProgressBar.h
  * Author:Ln_Jan
  * Version:v1.0.0
  * Date:2019-04-16
  * Description:流程进度条控件
  * Others:
  * Function List:
  * History:
  *
**********************************************************************************/
#ifndef QFLOWPROGRESSBAR_H
#define QFLOWPROGRESSBAR_H
#include "QFlowProgressBase.h"
#include <QRect>
#include <QVector>

class QFlowProgressBar:public QFlowProgressBase
{
public:
    enum Styles{PROGRESS_BAR_CIRCLE_1,PROGRESS_BAR_CIRCLE_2,PROGRESS_BAR_RECT};
    QFlowProgressBar(QWidget* parent=nullptr);

    QFlowProgressBar(const QStringList& strDetailList,const Styles& style=Styles::PROGRESS_BAR_CIRCLE_1,QWidget* parent=nullptr);

    /**
     * @brief SetProgressBarStyle 设置进度条的样式
     * @param style 样式设定
     * @details
     */
    void SetProgressBarStyle(Styles style){m_barStyle=style;}
    /**
     * @brief GetProgressBarStyle 获取进度条的样式
     * @return
     */
    Styles GetProgressBarStyle(){return m_barStyle;}

    void SetFinishedNumberColor(const QColor& color){m_clrFinishNumber=color;}

    const QColor& GetFinishedNumberColor(){return m_clrFinishNumber;}

    void SetStepMessageList(const QStringList& list) override;
protected:
    void paintEvent(QPaintEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void CalculeStepIconRect(QVector<QRect>& vec);
private:
    Styles m_barStyle;
    QColor m_clrFinishNumber;
    QVector<QRect> m_vecStepIconRect;

    void DrawCircle1Style(QPainter& painter);
    void DrawCircle2Style(QPainter& painter);
    void DrawRectangleStyle(QPainter& painter);
    void Init();
};

#endif // QFLOWPROGRESSBAR_H
