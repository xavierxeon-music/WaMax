#include "ImageDisplay.h"

#include <QPainter>

#include "Rainbow.h"

ImageDisplay::ImageDisplay(QQuickItem* parent)
   : QQuickPaintedItem(parent)
   , memorySubscriber(false)
   , image()
   , isInitialised(false)
{
}

ImageDisplay::~ImageDisplay()
{
}

void ImageDisplay::detach()
{
   memorySubscriber.detach();
}

void ImageDisplay::attach()
{
   if (memorySubscriber.isUpdated() || !isInitialised)
   {
      isInitialised = true;
      const QSize size = memorySubscriber.getSize();
      image = QImage(memorySubscriber.imageMemory, size.width(), size.height(), QImage::Format_ARGB32);
      //qDebug() << "Server image" << memorySubscriber.imageMemory << size << image.size() << image.isNull();
   }
   update();
}

void ImageDisplay::paint(QPainter* painter)
{
   const QColor bgColor = Rainbow::getColor().darker(300);
   painter->fillRect(QRect(0, 0, width(), height()), bgColor);

   if (image.isNull())
      return;

   painter->drawImage(QPoint(0, 0), image);
}
