#ifndef QFLOWPROGRESSBASE_H
#define QFLOWPROGRESSBASE_H
#include <QLabel>
#include <QStringList>
#include <QColor>

typedef void(*OnStepClickListener)(int);

class QFlowProgressBase:public QLabel
{
    Q_OBJECT
public:
    QFlowProgressBase(QWidget* parent=nullptr);
    QFlowProgressBase(const QStringList& strDetailList,QWidget* parent=nullptr);

    int GetStepNums()
    {
        if(m_strStepList.empty())
        {
            return 0;
        }
        return m_strStepList.size();
    }

    /**
     * @brief SetStepMessageList 设置进度条的文字描述
     * @param list
     */
    virtual void SetStepMessageList(const QStringList& list){m_strStepList=list;}
    /**
     * @brief GetStepMessageList 获取进度条的文字描述
     * @return
     */
    const QStringList& GetStepMessageList(){return m_strStepList;}
    /**
     * @brief NextStep 跳转到下一步
     */
    void NextStep()
    {
        if(m_nCurrentStep<GetStepNums()+1)
        {
            m_nCurrentStep++;
            update();
        }
    }
    /**
     * @brief BackToPrevStep 返回到上一步
     */
    void BackToPrevStep()
    {
        if(m_nCurrentStep>0)
        {
            m_nCurrentStep--;
            update();
        }
    }

    void ChangeCurrentStep(int nStep)
    {
        if(nStep>=0 && nStep<=GetStepNums())
        {
            m_nCurrentStep=nStep;
            update();
        }
    }

    /**
     * @brief GetCurrentStep 获取当前的进度
     * @return
     */
    int GetCurrentStep(){return m_nCurrentStep;}

    void SetAutoTextWidth(bool bAuto){m_bIsAutoTextWidth=bAuto;}

    bool GetAutoTextWidth(){return m_bIsAutoTextWidth;}

    void SetBackgroundColor(const QColor& color){m_clrBackground=color;}

    const QColor& GetBackgroundColor(){return m_clrBackground;}

    void SetFinishedBackgroundColor(const QColor& color){m_clrFinishBackground=color;}

    const QColor& GetFinishedBackgroundColor(){return m_clrFinishBackground;}

    void SetOnStepClickListener(OnStepClickListener l){m_listener=l;}

    OnStepClickListener GetOnStepClickListener(){return m_listener;}

signals:
    void signals_stepClicked(int);

protected:
    virtual void paintEvent(QPaintEvent* event)=0;

    virtual QSize GetDrawTextSize(const QString& strText,const QFont& ft);

    /**
     * @brief SplitTextFixWidth 根据给定的长度裁剪字符串
     * @param strText
     * @param ft
     * @param nMaxWidth
     * @return
     */
    virtual QString SplitTextFixWidth(const QString& strText,const QFont& ft,int nMaxWidth);
private:
    QStringList m_strStepList;
    int m_nCurrentStep;
    bool m_bIsAutoTextWidth;
    QColor m_clrBackground;
    QColor m_clrFinishBackground;
    OnStepClickListener m_listener;

    void Init();
};

#endif // QFLOWPROGRESSBASE_H
