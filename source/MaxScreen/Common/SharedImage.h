#ifndef SharedImageH
#define SharedImageH

#include <QColor>
#include <QFile>
#include <QImage>
#include <QSharedMemory>

#include "ImageSize.h"

class SharedImage
{
public:
   SharedImage(bool isPublisher, bool useFile = false);
   ~SharedImage();

public:
   QImage create();
   QImage createNew(const int width, const int height);
   void detach();

   QSize getSize() const;
   bool isUpdated(); // returns true if the size of the shared image is different from the current size

public:
   uchar* imageMemory;

private:
   struct Header
   {
      int width = 0;
      int height = 0;
   };

   static const int headerSize = sizeof(Header);
   static const int colorSize = 4 * sizeof(uchar);

private:
   void resize(const int width, const int height);

private:
   const bool isPublisher;
   const bool useFile;

   QFile sharedFile;
   QSharedMemory sharedMemoryHeader;
   QSharedMemory sharedMemoryImage;

   Header* header;
   int imageSize;
};

#ifndef SharedImageHPP
#include "SharedImage.hpp"
#endif // NOT SharedImageHPP

#endif // NOT SharedImageH
