/*********************************************************************************
  * Copyright(C) 2019,保留所有权利。( All rights reserved. )
  * FileName:QCardDialog.h
  * Author:Ln_Jan
  * Version:v1.0.0
  * Date:2019-04-16
  * Description:卡片式对话框
  * Others:
  * Function List:
  * History:
  *
**********************************************************************************/
#ifndef QCARDDIALOG_H
#define QCARDDIALOG_H
#include <QDialog>
#include <QColor>
#include <QRegion>

class QCardDialog : public QDialog
{
public:
    QCardDialog(QWidget* parent=nullptr);
    ~QCardDialog();
    void SetRectRadius(int nRadius){m_nRectangleRadius=nRadius;}
    int GetRectRadius(){return m_nRectangleRadius;}
    void SetBackgroundColor(const QColor& color){m_backgroundColor=color;}
    const QColor& GetBackgroundColor(){return m_backgroundColor;}
protected:
    void paintEvent(QPaintEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void enterEvent(QEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
private:
    bool m_bIsCloseIconShow;
    int m_nRectangleRadius;
    QColor m_backgroundColor;
    QRegion m_btnCloseRegion;
    QPoint m_ptMousePress;
    bool m_bIsWindowMoveFlag;
};

#endif // QCARDDIALOG_H
