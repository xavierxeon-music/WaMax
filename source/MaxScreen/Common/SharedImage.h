#ifndef SharedImageH
#define SharedImageH

#include <QColor>
#include <QFile>

#include "Size.h"

class SharedImage
{
public:
   SharedImage(bool isPublisher);

public:
   void create(const int width, const int height, const QColor& color = QColor(255, 255, 255));
   void createFromFile(const QString& fileName, const Size& size);
   void saveToFile(const QString& fileName);

   const Size& getSize() const;
   bool verify(); // returns true if the size of the shared image is different from the current size

   QColor getColor(int x, int y) const;
   void setColor(int x, int y, const QColor& color);

private:
   int index(int x, int y) const;
   void resize(const int width, const int height);

private:
   bool isPublisher;
   Size size;
   QFile sharedFile;
   uchar* fileMemory;
};

#ifndef SharedImageHPP
#include "SharedImage.hpp"
#endif // NOT SharedImageHPP

#endif // NOT SharedImageH
