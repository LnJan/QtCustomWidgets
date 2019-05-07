#ifndef QPIXMAPOBJECT_H
#define QPIXMAPOBJECT_H

#include <QObject>
#include <QPixmap>

class QPixmapObject : public QObject
{
    Q_OBJECT
public:
    explicit QPixmapObject(const QPixmap& pixmap,QObject *parent = nullptr);
    const QPixmap& GetPixmap(){return m_pixmap;}

signals:

public slots:

private:
    QPixmap m_pixmap;
};

#endif // QPIXMAPOBJECT_H
