#ifndef SharedImageHPP
#define SharedImageHPP

#include "SharedImage.h"

#include <QImage>

#include <Convertor.h>

#include "Data.h"

static Convertor<int> convertor;

inline SharedImage::SharedImage(bool isPublisher)
   : isPublisher(isPublisher)
   , sharedFile(ScreenServer::compileSharedFileName("block"))
   , header(nullptr)
   , imageSize(0)
   , imageMemory(nullptr)
{
   if (!sharedFile.open(QIODevice::ReadWrite))
      qDebug() << "unable to open shared file";

   if (isPublisher && sharedFile.size() < headerSize)
   {
      sharedFile.resize(headerSize);
      sharedFile.flush();
   }

   header = reinterpret_cast<Header*>(sharedFile.map(0, headerSize));
   imageMemory = nullptr;

   if (!isPublisher)
      verify();
}

inline SharedImage::~SharedImage()
{
   sharedFile.unmap(reinterpret_cast<uchar*>(header));

   if (imageMemory)
      sharedFile.unmap(imageMemory);
}

inline void SharedImage::create(const int width, const int height, const QColor& color)
{
   if (!isPublisher)
      return;

   resize(width, height);

   for (int x = 0; x < width; x++)
   {
      for (int y = 0; y < height; y++)
      {
         setColor(x, y, color);
      }
   }
}

inline void SharedImage::createFromFile(const QString& fileName, const QSize& size)
{
   if (!isPublisher)
      return;

   QImage image(fileName);
   if (fileName.isNull())
      return;

   if (size.isValid())
      image = image.scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation);

   resize(image.width(), image.height());

   for (int x = 0; x < image.width(); x++)
   {
      for (int y = 0; y < image.height(); y++)
      {
         const QColor color = image.pixelColor(x, y);
         setColor(x, y, color);
      }
   }
}

inline void SharedImage::saveToFile(const QString& fileName)
{
   QImage image(header->width, header->height, QImage::Format_RGB32);

   for (int x = 0; x < header->width; x++)
   {
      for (int y = 0; y < header->height; y++)
      {
         const QColor color = getColor(x, y);
         image.setPixelColor(x, y, color);
      }
   }

   image.save(fileName);
}

inline QSize SharedImage::getSize() const
{
   return QSize(header->width, header->height);
}

inline bool SharedImage::verify()
{
   if (isPublisher)
      return false;

   if (0 == sharedFile.size())
      return false;

   const int newimageSize = header->width * header->height * colorSize;
   if (newimageSize == imageSize)
      return false;

   if (imageMemory)
      sharedFile.unmap(imageMemory);

   imageSize = newimageSize;
   imageMemory = sharedFile.map(headerSize, imageSize);

   return true;
}

inline QColor SharedImage::getColor(int x, int y) const
{
   const int pos = index(x, y);
   const uchar red = imageMemory[pos + 0];
   const uchar green = imageMemory[pos + 1];
   const uchar blue = imageMemory[pos + 2];

   return QColor(red, green, blue);
}

inline void SharedImage::setColor(int x, int y, const QColor& color)
{
   const int pos = index(x, y);
   imageMemory[pos + 0] = color.red();
   imageMemory[pos + 1] = color.green();
   imageMemory[pos + 2] = color.blue();
}

inline int SharedImage::index(int x, int y) const
{
   int i = x + (y * header->width);
   return headerSize + i * colorSize;
}

inline void SharedImage::resize(const int width, const int height)
{
   if (imageMemory)
      sharedFile.unmap(imageMemory);

   header->width = width;
   header->height = height;
   imageSize = (width * height * colorSize);

   const int fileSize = headerSize + imageSize;
   sharedFile.resize(fileSize);
   sharedFile.flush();

   imageMemory = sharedFile.map(headerSize, imageSize);
}

#endif // NOT SharedImageHPP
