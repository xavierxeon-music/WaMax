#include "ImageDisplay.h"

#include <QPainter>
#include <QTimer>

#include "Rainbow.h"

ImageDisplay::ImageDisplay(QQuickItem* parent)
   : QQuickPaintedItem(parent)
   , subscriberImage(false)
{
   QTimer* updateTimer = new QTimer(this);

   auto updateFunction = [this]()
   {
      this->update();
   };
   connect(updateTimer, &QTimer::timeout, updateFunction);

   updateTimer->start(100);
}

ImageDisplay::~ImageDisplay()
{
}

void ImageDisplay::paint(QPainter* painter)
{
   const QColor bgColor = Rainbow::getColor().darker(300);
   painter->fillRect(QRect(0, 0, width(), height()), bgColor);

   subscriberImage.verify();
   const int imageWidth = subscriberImage.getSize().getWidth();
   const int imageHeight = subscriberImage.getSize().getHeight();

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
