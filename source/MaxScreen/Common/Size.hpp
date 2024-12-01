#ifndef SizeHPP
#define SizeHPP

#include "Size.h"

#include <QDebug>

inline Size::Size()
   : width(0)
   , height(0)
{
}

inline void Size::update(const int _width, const int _height)
{
   width = _width;
   height = _height;
}

inline void Size::dump(QDataStream& stream)
{
   static const char marker = 's';
   stream << marker << width << height;
}

inline void Size::load(QDataStream& stream)
{
   stream >> width;
   stream >> height;

   //qDebug() << width << height;
}

inline const int& Size::getWidth() const
{
   return width;
}

inline const int& Size::getHeight() const
{
   return height;
}

#endif // NOT SizeHPP
