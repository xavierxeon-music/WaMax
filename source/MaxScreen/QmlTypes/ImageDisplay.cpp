#include "ImageDisplay.h"

#include <QPainter>

#include "Rainbow.h"

ImageDisplay::ImageDisplay(QQuickItem* parent)
   : QQuickPaintedItem(parent)
   , subscriberImage(false)
{
}

ImageDisplay::~ImageDisplay()
{
}

void ImageDisplay::detach()
{
}

void ImageDisplay::attach()
{
   subscriberImage.verify();
   update();
}

void ImageDisplay::paint(QPainter* painter)
{
   const QColor bgColor = Rainbow::getColor().darker(300);
   painter->fillRect(QRect(0, 0, width(), height()), bgColor);

   const int imageWidth = subscriberImage.getSize().width();
   const int imageHeight = subscriberImage.getSize().height();

   int xOffset = (imageWidth < width()) ? 0.5 * (width() - imageWidth) : 0;
   int yOffset = (imageHeight < height()) ? 0.5 * (height() - imageHeight) : 0;

   for (int x = 0; x < imageWidth; x++)
   {
      for (int y = 0; y < imageHeight; y++)
      {
         const QColor color = subscriberImage.getColor(x, y);
         painter->setPen(color);
         painter->drawPoint(x + xOffset, y + yOffset);
      }
   }
}
