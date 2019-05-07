#include "QFlowProgressBase.h"

QFlowProgressBase::QFlowProgressBase(QWidget* parent):QLabel(parent),m_nCurrentStep(0),m_bIsAutoTextWidth(false),
    m_listener(nullptr)
{
    Init();
}

QFlowProgressBase::QFlowProgressBase(const QStringList& strDetailList,QWidget* parent):QLabel(parent),
    m_strStepList(strDetailList),m_nCurrentStep(0),m_bIsAutoTextWidth(false),m_listener(nullptr)
{
    Init();
}


void QFlowProgressBase::Init()
{
    m_clrBackground.setRgb(200,200,200);
    m_clrFinishBackground.setRgb(40,220,20);
}

QSize QFlowProgressBase::GetDrawTextSize(const QString& strText,const QFont& ft)
{
    QFontMetrics metrics(ft);
    QStringList ls=strText.split(QRegExp("\r\n|\n"));
    int nWidthInPixel=0;
    for(int i=0;i<ls.size();i++)
    {
        int tmpWidth=metrics.horizontalAdvance(ls.at(i));
        nWidthInPixel=tmpWidth>nWidthInPixel?tmpWidth:nWidthInPixel;
    }
    int nHeightInPixel=(metrics.height())*ls.length()+(metrics.leading()*(ls.length()-1));
    return QSize(nWidthInPixel,nHeightInPixel);
}

QString QFlowProgressBase::SplitTextFixWidth(const QString& strText,const QFont& ft,int nMaxWidth)
{
    QString strTextCpy=strText;
    strTextCpy.replace(QRegExp("\r\n|\n"),"");
    QFontMetrics metrics(ft);
    int nTextWidth=metrics.horizontalAdvance(strTextCpy);
    if(nTextWidth<=nMaxWidth)
    {
        return strTextCpy;
    }
    QVector<int> vecSplitIndex;
    vecSplitIndex.reserve(10);
    int nSplitFragmentNums=nTextWidth/nMaxWidth+1;
    int nSplitFragmentLen=strTextCpy.length()/nSplitFragmentNums;

    vecSplitIndex.push_back(0);
    for(int i=1;i<nSplitFragmentNums;i++)
    {
        vecSplitIndex.push_back(nSplitFragmentLen*i);
    }
    vecSplitIndex.push_back(strTextCpy.length());

    for(int j=0;j<vecSplitIndex.size()-1;j++)
    {
        int nLen=vecSplitIndex.at(j+1)-vecSplitIndex.at(j);
        do
        {
            QString strTempCheck=strTextCpy.mid(vecSplitIndex.at(j),nLen);
            int nTmpTextWidth=metrics.horizontalAdvance(strTempCheck);
            if(nTmpTextWidth>nMaxWidth)
            {
                nLen--;
            }
            else
            {
                break;
            }
        }while(1);
        vecSplitIndex[j+1]=vecSplitIndex[j]+nLen;
    }

    if(vecSplitIndex[vecSplitIndex.size()-1]!=strTextCpy.length())
    {
        vecSplitIndex.push_back(strTextCpy.length());
    }

    for(int i=1;i<vecSplitIndex.size()-1;i++)
    {
        strTextCpy.insert(vecSplitIndex[i],"\n");
    }
    return strTextCpy;
}
