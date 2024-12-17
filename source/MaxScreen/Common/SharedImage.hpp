#ifndef SharedImageHPP
#define SharedImageHPP

#include "SharedImage.h"

#include <Shared.h>

using ScreenServer = Shared<"MaxScreen">;

inline SharedImage::SharedImage(bool isPublisher, bool useFile)
   : isPublisher(isPublisher)
   , useFile(useFile)
   , imageMemory(nullptr)
   , sharedFile(ScreenServer::compileSharedFileName("data"))
   , sharedMemoryHeader(ScreenServer::appName() + "_header")
   , sharedMemoryImage(ScreenServer::appName() + "_image")
   , header(nullptr)
   , imageSize(0)
{
   if (useFile)
   {
      if (!sharedFile.open(QIODevice::ReadWrite))
         qDebug() << "unable to open shared file";

      if (isPublisher && sharedFile.size() < headerSize)
      {
         sharedFile.resize(headerSize);
         sharedFile.flush();
      }

      header = reinterpret_cast<Header*>(sharedFile.map(0, headerSize));
   }
   else
   {
      if (!sharedMemoryHeader.create(headerSize))
      {
         if (QSharedMemory::AlreadyExists == sharedMemoryHeader.error())
            sharedMemoryHeader.attach();
      }

      header = reinterpret_cast<Header*>(sharedMemoryHeader.data());
   }

   imageMemory = nullptr;
   isUpdated();
}

inline SharedImage::~SharedImage()
{
   if (useFile)
   {
      sharedFile.unmap(reinterpret_cast<uchar*>(header));

      if (imageMemory)
         sharedFile.unmap(imageMemory);
   }
   else
   {
      sharedMemoryHeader.detach();
      sharedMemoryImage.detach();
   }
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

inline void SharedImage::detach()
{
   if (useFile)
   {
      // do nothing
   }
   else
   {
      sharedMemoryImage.detach();
   }
}

inline QSize SharedImage::getSize() const
{
   return QSize(header->width, header->height);
}

inline bool SharedImage::isUpdated()
{
   if (isPublisher)
      return false;

   if (useFile && 0 == sharedFile.size())
      return false;

   const int newimageSize = header->width * header->height * colorSize;

   if (useFile)
   {
      if (newimageSize == imageSize)
         return false;

      if (imageMemory)
         sharedFile.unmap(imageMemory);

      imageSize = newimageSize;
      imageMemory = sharedFile.map(headerSize, imageSize);
   }
   else
   {
      if (sharedMemoryImage.isAttached() && newimageSize == imageSize)
         return false;

      sharedMemoryImage.attach();
      imageMemory = reinterpret_cast<uchar*>(sharedMemoryImage.data());

      //qDebug() << __FUNCTION__ << sharedMemoryHeader.isAttached() << sharedMemoryHeader.size() << sharedMemoryImage.isAttached() << sharedMemoryImage.size() << sharedMemoryImage.error();
   }

   return true;
}

inline void SharedImage::resize(const int width, const int height)
{
   header->width = width;
   header->height = height;
   imageSize = (width * height * colorSize);

   if (useFile)
   {
      if (imageMemory)
         sharedFile.unmap(imageMemory);

      const int fileSize = headerSize + imageSize;
      sharedFile.resize(fileSize);
      sharedFile.flush();

      imageMemory = sharedFile.map(headerSize, imageSize);
   }
   else
   {
      sharedMemoryImage.detach();

      if (!sharedMemoryImage.create(imageSize))
      {
         if (QSharedMemory::AlreadyExists == sharedMemoryHeader.error())
            sharedMemoryImage.attach();
      }

      imageMemory = reinterpret_cast<uchar*>(sharedMemoryImage.data());

      //qDebug() << __FUNCTION__ << sharedMemoryHeader.isAttached() << sharedMemoryHeader.size() << sharedMemoryImage.isAttached() << sharedMemoryImage.size() << sharedMemoryImage.error();
   }
}

#endif // NOT SharedImageHPP
