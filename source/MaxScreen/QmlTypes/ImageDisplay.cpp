#include "ImageDisplay.h"

#include <QPainter>
#include <QQmlContext>

#include "Rainbow.h"

// see https://doc.qt.io/qt-6/qtcore-ipc-sharedmemory-example.html

ImageDisplay::ImageDisplay(QQuickItem* parent)
   : QQuickPaintedItem(parent)
{
}

ImageDisplay::~ImageDisplay()
{
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

   int xOffset = (buffer.width() < width()) ? 0.5 * (width() - buffer.width()) : 0;
   int yOffset = (buffer.height() < height()) ? 0.5 * (height() - buffer.height()) : 0;

   painter->drawImage(QPoint(xOffset, yOffset), buffer);
}
