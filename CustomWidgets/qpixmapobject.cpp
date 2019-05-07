#include "qpixmapobject.h"

QPixmapObject::QPixmapObject(const QPixmap& pixmap,QObject *parent) : QObject(parent)
  ,m_pixmap(pixmap)
{

}
