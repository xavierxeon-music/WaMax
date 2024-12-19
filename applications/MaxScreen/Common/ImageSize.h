#ifndef ImageSizeH
#define ImageSizeH

#include <QDataStream>
#include <QSize>

class ImageSize
{
public:
   ImageSize();

public:
   bool isValid() const;
   void update(const int width, const int height);
   void dump(QDataStream& stream);
   void load(QDataStream& stream);
   const int& getWidth() const;
   const int& getHeight() const;
   operator QSize() const;

private:
   int width;
   int height;
};

#ifndef ImageSizeHPP
#include "ImageSize.hpp"
#endif // NOT ImageSizeHPP

#endif // NOT ImageSizeH
