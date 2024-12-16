#ifndef ImageSizeHPP
#define ImageSizeHPP

#include "ImageSize.h"

#include <QDebug>

#include "Marker.h"

inline ImageSize::ImageSize()
   : width(0)
   , height(0)
{
}

inline bool ImageSize::isValid() const
{
   return (0 != width && 0 != height);
}

inline void ImageSize::update(const int _width, const int _height)
{
   width = _width;
   height = _height;
}

inline void ImageSize::dump(QDataStream& stream)
{
   stream << Marker::ScreenSize << width << height;
}

inline void ImageSize::load(QDataStream& stream)
{
   stream >> width;
   stream >> height;

   //qDebug() << width << height;
}

inline const int& ImageSize::getWidth() const
{
   return width;
}

inline const int& ImageSize::getHeight() const
{
   return height;
}

inline ImageSize::operator QSize() const
{
   return QSize(width, height);
}

#endif // NOT ImageSizeHPP
