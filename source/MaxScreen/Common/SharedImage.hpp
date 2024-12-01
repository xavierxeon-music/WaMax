#ifndef SharedImageHPP
#define SharedImageHPP

#include "SharedImage.h"

#include <QImage>

#include <Convertor.h>

#include "Data.h"

static Convertor<int> convertor;

static const int headerSize = 2 * sizeof(int);
static const int colorSize = 3 * sizeof(uchar);

inline SharedImage::SharedImage(bool isPublisher)
   : isPublisher(isPublisher)
   , size()
   , sharedFile(ScreenServer::compileSharedFileName("block"))
   , fileMemory(nullptr)
{
   if (!sharedFile.open(QIODevice::ReadWrite))
      qDebug() << "unable to open shared file";

   if (isPublisher && sharedFile.size() < headerSize)
   {
      bool test = sharedFile.resize(headerSize);
      sharedFile.flush();
   }

   fileMemory = sharedFile.map(0, headerSize);

   if (!isPublisher)
      verify();
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

inline void SharedImage::createFromFile(const QString& fileName)
{
   if (!isPublisher)
      return;

   QImage image(fileName);
   if (fileName.isNull())
      return;

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
   QImage image(size.getWidth(), size.getHeight(), QImage::Format_RGB32);

   for (int x = 0; x < size.getWidth(); x++)
   {
      for (int y = 0; y < size.getHeight(); y++)
      {
         const QColor color = getColor(x, y);
         image.setPixelColor(x, y, color);
      }
   }

   image.save(fileName);
}

inline const Size& SharedImage::getSize() const
{
   return size;
}

inline bool SharedImage::verify()
{
   if (isPublisher)
      return false;

   auto getParam = [&](int offset)
   {
      for (int pos : {0, 1, 2, 3})
         convertor.bytes[pos] = fileMemory[offset + pos];

      return convertor.data;
   };

   const int width = getParam(0);
   const int height = getParam(sizeof(int));

   if (size.getWidth() == width && size.getHeight() == height)
      return false;

   size.update(width, height);

   if (fileMemory)
      sharedFile.unmap(fileMemory);

   const int fileSize = headerSize + (width * height * colorSize);
   fileMemory = sharedFile.map(0, fileSize);

   return true;
}

inline QColor SharedImage::getColor(int x, int y) const
{
   const int pos = index(x, y);
   const uchar red = fileMemory[pos + 0];
   const uchar green = fileMemory[pos + 1];
   const uchar blue = fileMemory[pos + 2];

   return QColor(red, green, blue);
}

inline void SharedImage::setColor(int x, int y, const QColor& color)
{
   const int pos = index(x, y);
   fileMemory[pos + 0] = color.red();
   fileMemory[pos + 1] = color.green();
   fileMemory[pos + 2] = color.blue();
}

inline int SharedImage::index(int x, int y) const
{
   return headerSize + x + (y * size.getWidth());
}

inline void SharedImage::resize(const int width, const int height)
{
   size.update(width, height);

   if (fileMemory)
      sharedFile.unmap(fileMemory);

   const int fileSize = headerSize + (width * height * colorSize);
   sharedFile.resize(fileSize);
   sharedFile.flush();

   fileMemory = sharedFile.map(0, fileSize);

   auto setParam = [&](int value, int offset)
   {
      convertor.data = value;
      for (int pos : {0, 1, 2, 3})
         fileMemory[offset + pos] = convertor.bytes[pos];
   };

   setParam(width, 0);
   setParam(height, sizeof(int));
}

#endif // NOT SharedImageHPP
