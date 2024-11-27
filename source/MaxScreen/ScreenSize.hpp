#ifndef ScreenSizeHPP
#define ScreenSizeHPP

#include "ScreenSize.h"

#include <QDebug>

inline ScreenSize::ScreenSize()
   : width(0)
   , height(0)
{
}

inline void ScreenSize::update(const int _width, const int _height)
{
   width = _width;
   height = _height;
}

inline void ScreenSize::dump(QDataStream& stream)
{
   static const char marker = 's';
   stream << marker << width << height;
}

inline void ScreenSize::load(QDataStream& stream)
{
   stream >> width;
   stream >> height;

   //qDebug() << width << height;
}

inline const int& ScreenSize::getWidth() const
{
   return width;
}

inline const int& ScreenSize::getHeight() const
{
   return height;
}

#endif // NOT ScreenSizeHPP
