#ifndef QTOAST_H
#define QTOAST_H

#include <QWidget>
#include <QString>
#include <QSize>
#include <QFont>

class QToast : public QWidget
{
    Q_OBJECT

public:
    static int LENGTH_LONG;
    static int LENGTH_SHORT;
    enum TOAST_POS{TOP,CENTER,BOTTOM};
    static QToast* CreateToast(QString strContent,int nToastInterval,QWidget *parent = nullptr);
    ~QToast();
    void SetTextFont(const QFont& font){m_drawFont=font;}
    const QFont& GetTextFont(){return m_drawFont;}
    void SetToastPos(TOAST_POS pos);
    void SetAutoDelete(bool b){m_bAutoDelete=b;}
signals:
    void signals_finished();
protected:
    virtual QSize CalculateTextSize();
    virtual void Init();
    void paintEvent(QPaintEvent *event);
    void showEvent(QShowEvent *event);
    void timerEvent(QTimerEvent *event);
private:
    QString m_strContent;
    int m_nToastInterval;
    int m_nCurrentWindowOpacity;
    int m_nCurrentStayTime;
    int m_nStatus;
    bool m_bAutoDelete;
    QFont m_drawFont;

    QToast(QString strContent,int nToastInterval,QWidget *parent = nullptr);
};

#endif // QTOAST_H
