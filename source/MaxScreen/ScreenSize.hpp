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

inline void ScreenSize::save(QDataStream& stream)
{
   static const char marker = 's';
   stream << marker << width << height;
}

inline void ScreenSize::load(QDataStream& stream)
{
   int _width = 0;
   stream >> _width;

   int _height = 0;
   stream >> _height;

   qDebug() << _width << _height;
}

#endif // NOT ScreenSizeHPP
