#ifndef TouchPointHPP
#define TouchPointHPP

#include "TouchPoint.h"

#include <QMetaProperty>
#include <QObject>
#include <QVariant>
#include <QVector2D>

#include "Marker.h"

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
   static bool doneOnce = true;
   if (!doneOnce)
   {
      const QMetaObject* metaObject = qmlTouchPoint->metaObject();
      for (int p = 0; p < metaObject->propertyCount(); p++)
      {
         const QMetaProperty metaProperty = metaObject->property(p);
         qDebug() << metaProperty.name();
      }
      doneOnce = true;
   }

   const int pointId = qmlTouchPoint->property("pointId").toInt();
   if (!contains(pointId))
      insert(pointId, TouchPoint());

   TouchPoint& touchPoint = find(pointId).value();
   touchPoint.pressed = qmlTouchPoint->property("pressed").toBool();

   touchPoint.x = qmlTouchPoint->property("x").toDouble();
   touchPoint.y = qmlTouchPoint->property("y").toDouble();
   touchPoint.startX = qmlTouchPoint->property("startX").toDouble();
   touchPoint.startY = qmlTouchPoint->property("startY").toDouble();

   touchPoint.pressure = qmlTouchPoint->property("pressure").toDouble();
   touchPoint.rotation = qmlTouchPoint->property("rotation").toDouble();
   touchPoint.velocity = qmlTouchPoint->property("velocity").value<QVector2D>().toPointF();
   touchPoint.area = qmlTouchPoint->property("area").toRectF();
}

inline void TouchPoint::Map::dump(QDataStream& stream)
{
   stream << Marker::TouchPoint << count();

   QList<int> deleteList;
   for (Map::const_iterator it = constBegin(); it != constEnd(); it++)
   {
      const TouchPoint& touchPoint = it.value();
      stream << it.key();
      stream << touchPoint.pressed;

      stream << touchPoint.x;
      stream << touchPoint.y;
      stream << touchPoint.startX;
      stream << touchPoint.startY;

      stream << touchPoint.pressure;
      stream << touchPoint.rotation;
      stream << touchPoint.velocity;
      stream << touchPoint.area;

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

      stream >> touchPoint.pressure;
      stream >> touchPoint.rotation;
      stream >> touchPoint.velocity;
      stream >> touchPoint.area;

      insert(id, touchPoint);
   }
}

// touch point

inline TouchPoint::TouchPoint()
   : pressed(false)
   , x(0.0)
   , y(0.0)
   , startX(0.0)
   , startY(0.0)
   , pressure(0.0)
   , rotation(0.0)
   , velocity()
   , area()
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

inline const double& TouchPoint::getPressure() const
{
   return pressure;
}

inline const double& TouchPoint::getRotation() const
{
   return rotation;
}

inline const QPointF& TouchPoint::getVelocity() const
{
   return velocity;
}

inline const QRectF& TouchPoint::getAreaRect() const
{
   return area;
}

inline double TouchPoint::getArea() const
{
   return area.width() * area.height();
}

#endif // NOT TouchPointHPP
