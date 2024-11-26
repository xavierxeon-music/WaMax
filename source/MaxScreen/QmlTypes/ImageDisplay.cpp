#include "ImageDisplay.h"

#include <QPainter>
#include <QQmlContext>

#include "Rainbow.h"

ImageDisplay* ImageDisplay::me = nullptr;

ImageDisplay::ImageDisplay(QQuickItem* parent)
   : QQuickPaintedItem(parent)
{
   me = this;
}

ImageDisplay::~ImageDisplay()
{
   me = nullptr;
}

void ImageDisplay::push(const QImage& image)
{
   if (!me)
      return;

   me->setBuffer(image);
}

void ImageDisplay::setBuffer(const QImage& image)
{
   //image.save("/Users/waspe/tmp/test.png");

   buffer = image.scaled(width(), height(), Qt::KeepAspectRatio, Qt::FastTransformation);
   update();
}

void ImageDisplay::paint(QPainter* painter)
{
   const QColor bgColor = Rainbow::getColor().darker(300);
   painter->fillRect(QRect(0, 0, width(), height()), bgColor);

   int x = (buffer.width() < width()) ? 0.5 * (width() - buffer.width()) : 0;
   int y = (buffer.height() < height()) ? 0.5 * (height() - buffer.height()) : 0;

   painter->drawImage(QPoint(x, y), buffer);
}
