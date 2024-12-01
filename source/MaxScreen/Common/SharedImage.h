#ifndef SharedImageH
#define SharedImageH

#include <QColor>
#include <QFile>

#include "Size.h"

class SharedImage
{
public:
   SharedImage(bool isPublisher);
   ~SharedImage();

public:
   void create(const int width, const int height, const QColor& color = QColor(255, 255, 255));
   void createFromFile(const QString& fileName, const QSize& size);
   void saveToFile(const QString& fileName);

   QSize getSize() const;
   bool verify(); // returns true if the size of the shared image is different from the current size

   QColor getColor(int x, int y) const;
   void setColor(int x, int y, const QColor& color);

private:
   struct Header
   {
      int width = 0;
      int height = 0;
   };

   static const int headerSize = sizeof(Header);
   static const int colorSize = 4 * sizeof(uchar);

private:
   int index(int x, int y) const;
   void resize(const int width, const int height);

private:
   bool isPublisher;
   QFile sharedFile;
   Header* header;

   int imageSize;
   uchar* imageMemory;
};

#ifndef SharedImageHPP
#include "SharedImage.hpp"
#endif // NOT SharedImageHPP

#endif // NOT SharedImageH
