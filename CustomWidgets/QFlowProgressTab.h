#ifndef QFLOWPROGRESSTAB_H
#define QFLOWPROGRESSTAB_H
#include "QFlowProgressBase.h"
#include <QVector>
#include <QRect>

class QFlowProgressTab:public QFlowProgressBase
{
public:
    QFlowProgressTab(QWidget* parent=nullptr);
    QFlowProgressTab(const QStringList& strMessageList,QWidget* parent=nullptr);
    void SetNormalTextColor(const QColor& clr){m_clrNormalText=clr;}
    const QColor& GetNormalTextColor(){return m_clrNormalText;}
    void SetFinishedTextColor(const QColor& clr){m_clrFinishedText=clr;}
    const QColor& GetFinishedTextColor(){return m_clrFinishedText;}
    void SetNormalCircleColor(const QColor& clr){m_clrNormalCircle=clr;}
    const QColor& GetNormalCircleColor(){return m_clrNormalCircle;}
    void SetFinishedCircleColor(const QColor& clr){m_clrFinishedCircle=clr;}
    const QColor& GetFinishedCircleColor(){return m_clrFinishedCircle;}
    void SetNormalNumberColor(const QColor& clr){m_clrNormalNumber=clr;}
    const QColor& GetNormalNumberColor(){return m_clrNormalNumber;}
    void SetFinishedNumberColor(const QColor& clr){m_clrFinishedNumber=clr;}
    const QColor& GetFinishedNumberColor(){return m_clrFinishedNumber;}
protected:
    void paintEvent(QPaintEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void CalculeStepIconRect(QVector<QRect>& vec);;
private:
    void DrawTabStyle(QPainter& painter);
    void Init();

    QColor m_clrNormalText;
    QColor m_clrFinishedText;
    QColor m_clrNormalCircle;
    QColor m_clrFinishedCircle;
    QColor m_clrNormalNumber;
    QColor m_clrFinishedNumber;
};

#endif // QFLOWPROGRESSTAB_H
