#include "QToast.h"
#include <QPainter>
#include <QFontMetrics>
#include <QGuiApplication>
#include <QWindow>
#include <QScreen>
#include <QPen>
#include <QColor>
#include <QDebug>
#include <QTimerEvent>

int QToast::LENGTH_LONG=4000;
int QToast::LENGTH_SHORT=1500;
static const int TIMER_INTERVAL=50;

QToast::QToast(QString strContent,int nToastInterval,QWidget *parent)
    : QWidget(parent),m_strContent(strContent),m_nToastInterval(nToastInterval),
      m_nCurrentWindowOpacity(0),m_nCurrentStayTime(0),m_nStatus(0),m_bAutoDelete(true)
{
    Init();
}

QToast::~QToast()
{
    qDebug()<<"QToast delete";
}


QSize QToast::CalculateTextSize()
{
    QFontMetrics metrice(m_drawFont);
    QStringList ls=m_strContent.split(QRegExp("\r\n|\n"));
    int nWidthInPixel=0;
    for(int i=0;i<ls.size();i++)
    {
        int tmpWidth=metrice.horizontalAdvance(ls.at(i));
        nWidthInPixel=tmpWidth>nWidthInPixel?tmpWidth:nWidthInPixel;
    }
    int nHeightInPixel=metrice.height()*ls.length();
    return QSize(nWidthInPixel,nHeightInPixel);
}

void QToast::Init()
{
    setWindowFlags(Qt::FramelessWindowHint|Qt::SubWindow);
    setAttribute(Qt::WA_TranslucentBackground);
    m_drawFont.setPointSize(20);
    SetToastPos(TOAST_POS::BOTTOM);
}


void QToast::SetToastPos(TOAST_POS pos)
{
    //获取主屏幕分辨率
    QRect screenRect = QGuiApplication::screens().at(0)->geometry();
    QSize fontsize=CalculateTextSize();
    QSize windowSize(fontsize.width()+fontsize.width()*0.4,fontsize.height()+fontsize.height()*0.4);
    int windowsX=(screenRect.width()-windowSize.width())/2;
    int windowsY=0;
    if(pos==TOAST_POS::TOP)
    {
        windowsY=screenRect.height() / 6;
    }
    else if(pos==TOAST_POS::BOTTOM)
    {
        windowsY=screenRect.height() * 4 / 5;
    }
    else
    {
        windowsY=(screenRect.height()-windowSize.height())/2;
    }

    setGeometry(windowsX,windowsY,windowSize.width(),windowSize.height());
}

void QToast::paintEvent(QPaintEvent*)
{

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setFont(m_drawFont);
    QSize widgetSize=size();
    QPen textPen(Qt::PenStyle::SolidLine);
    textPen.setColor(QColor(255,255,255,m_nCurrentWindowOpacity*255/100));
    textPen.setJoinStyle(Qt::PenJoinStyle::RoundJoin);


    QPen emptyPen(Qt::PenStyle::NoPen);

    //painter.setPen(textPen);
    painter.setPen(emptyPen);
    painter.setBrush(QBrush(QColor(100,100,100,m_nCurrentWindowOpacity*255/100)));
    painter.drawRoundRect(widgetSize.width()*0.05,widgetSize.height()*0.05,widgetSize.width()*0.9,widgetSize.height()*0.9,10,10);
    painter.setPen(textPen);
    painter.drawText(QRect(widgetSize.width()*0.15,widgetSize.height()*0.1,widgetSize.width()*0.8,widgetSize.height()*0.8),m_strContent);
}

void QToast::showEvent(QShowEvent*)
{
    qDebug()<<"show event call";
    if(m_nStatus==0x00)
    {
        m_nCurrentStayTime=0;
        m_nCurrentWindowOpacity=0;
        startTimer(TIMER_INTERVAL);
        m_nStatus=0x01;
    }
}

void QToast::timerEvent(QTimerEvent* e)
{
    if(m_nStatus==0x01)
    {
        if(m_nCurrentWindowOpacity<100)
        {
            m_nCurrentWindowOpacity+=10;
            update();
        }
        else
        {
            m_nStatus=0x02;
        }
    }
    else if(m_nStatus==0x02)
    {
        if(m_nCurrentStayTime<m_nToastInterval)
        {
            m_nCurrentStayTime+=TIMER_INTERVAL;
        }
        else
        {
            m_nStatus=0x03;
        }
    }
    else if(m_nStatus==0x03)
    {
        if(m_nCurrentWindowOpacity>0)
        {
            m_nCurrentWindowOpacity-=10;
            update();
        }
        else
        {
            m_nStatus=0x04;
        }
    }
    else if(m_nStatus==0x04)
    {
        m_nStatus=0x00;
        emit signals_finished();
        killTimer(e->timerId());
        if(m_bAutoDelete)
        {
            this->deleteLater();
        }
    }
}

QToast* QToast::CreateToast(QString strContent,int nToastInterval,QWidget *parent)
{
    return new QToast(strContent,nToastInterval,parent);
}
