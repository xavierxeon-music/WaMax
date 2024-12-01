#ifndef SharedImageHPP
#define SharedImageHPP

#include "SharedImage.h"

#include <Shared.h>

using ScreenServer = Shared<"MaxScreen">;

inline SharedImage::SharedImage(bool isPublisher)
   : isPublisher(isPublisher)
   , imageMemory(nullptr)
   , sharedFile(ScreenServer::compileSharedFileName("data"))
   , header(nullptr)
   , imageSize(0)
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
      isUpdated();
}

inline SharedImage::~SharedImage()
{
   sharedFile.unmap(reinterpret_cast<uchar*>(header));

   if (imageMemory)
      sharedFile.unmap(imageMemory);
}

inline QImage SharedImage::create()
{
   return QImage(imageMemory, header->width, header->height, QImage::Format_ARGB32);
}

inline QImage SharedImage::createNew(const int width, const int height)
{
   if (!isPublisher)
      return QImage();

   resize(width, height);

   return create();
}

inline QSize SharedImage::getSize() const
{
   return QSize(header->width, header->height);
}

inline bool SharedImage::isUpdated()
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
