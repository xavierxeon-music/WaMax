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
}

inline void TouchPoint::Map::save(QDataStream& stream)
{
   static const char marker = 't';
   stream << marker << count();

   QList<int> deleteList;
   for (Map::const_iterator it = constBegin(); it != constEnd(); it++)
   {
      stream << it.key() << it.value().isPressed() << it.value().getX() << it.value().getY();

      if (!it.value().pressed)
         deleteList.append(it.key());
   }

   for (const int& key : deleteList)
      remove(key);
}

inline void TouchPoint::Map::load(QDataStream& stream)
{
   // clear();

   size_type counter = 0;
   stream >> counter;
   for (int index = 0; index < counter; index++)
   {
      int id = 0;
      stream >> id;

      bool pressed = false;
      stream >> pressed;

      double x = 0.0;
      stream >> x;

      double y = 0.0;
      stream >> y;

      qDebug() << id << pressed << x << y;
   }
}

// touch point

inline TouchPoint::TouchPoint()
   : pressed(false)
   , x(0.0)
   , y(0.0)
{
}

inline bool TouchPoint::isPressed() const
{
   return pressed;
}

inline const double& TouchPoint::getX() const
{
   return x;
}

inline const double& TouchPoint::getY() const
{
   return y;
}

#endif // NOT TouchPointHPP
