#ifndef TouchPointHPP
#define TouchPointHPP

#include "TouchPoint.h"

#include <QObject>
#include <QVariant>

// map

inline TouchPoint::Map::Map()
   : QMap<int, TouchPoint>()
{
}

inline void TouchPoint::Map::startUpdates()
{
   for (Map::iterator it = begin(); it != end(); it++)
   {
      it.value().pressed = false;
   }
}

inline void TouchPoint::Map::update(const QObject* qmlTouchPoint)
{
   const int pointId = qmlTouchPoint->property("pointId").toInt();
   if (!contains(pointId))
      insert(pointId, TouchPoint());

   TouchPoint& touchPoint = find(pointId).value();

   touchPoint.pressed = qmlTouchPoint->property("pressed").toBool();
   touchPoint.x = qmlTouchPoint->property("x").toDouble();
   touchPoint.y = qmlTouchPoint->property("y").toDouble();
   touchPoint.startX = qmlTouchPoint->property("startX").toDouble();
   touchPoint.startY = qmlTouchPoint->property("startY").toDouble();
}

inline void TouchPoint::Map::dump(QDataStream& stream)
{
   static const char marker = 't';
   stream << marker << count();

   QList<int> deleteList;
   for (Map::const_iterator it = constBegin(); it != constEnd(); it++)
   {
      const TouchPoint& touchPoint = it.value();
      //stream << it.key() << touchPoint.pressed << touchPoint.x << touchPoint.y << touchPoint.startX << touchPoint.startY;

      if (!touchPoint.pressed)
         deleteList.append(it.key());
   }

   for (const int& key : deleteList)
      remove(key);
}

inline void TouchPoint::Map::load(QDataStream& stream)
{
   clear();

   size_type counter = 0;
   stream >> counter;
   for (int index = 0; index < counter; index++)
   {
      int id = 0;
      stream >> id;

      TouchPoint touchPoint;
      stream >> touchPoint.pressed;
      stream >> touchPoint.x;
      stream >> touchPoint.y;
      stream >> touchPoint.startX;
      stream >> touchPoint.startY;

      insert(id, touchPoint);

      // qDebug() << id << touchPoint.pressed << touchPoint.x << touchPoint.y << touchPoint.startX << touchPoint.startY;
   }
}

// touch point

inline TouchPoint::TouchPoint()
   : pressed(false)
   , x(0.0)
   , y(0.0)
   , startX(0.0)
   , startY(0.0)
{
}

inline bool TouchPoint::isPressed() const
{
   return pressed;
}

inline const int& TouchPoint::getX() const
{
   return x;
}

inline const int& TouchPoint::getY() const
{
   return y;
}

inline const int& TouchPoint::getStartX() const
{
   return startX;
}

inline const int& TouchPoint::getStartY() const
{
   return startY;
}

#endif // NOT TouchPointHPP
