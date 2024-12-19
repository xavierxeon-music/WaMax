#ifndef TouchPointH
#define TouchPointH

#include <QMap>

#include <QDataStream>
#include <QPointF>
#include <QRectF>

class TouchPoint
{
public:
   class Map : public QMap<int, TouchPoint>
   {
   public:
      Map();

   public:
      void startUpdates();
      void update(const QObject* qmlTouchPoint);
      void dump(QDataStream& stream);
      void load(QDataStream& stream);
   };

public:
   TouchPoint();

public:
   bool isPressed() const;
   const int& getX() const;
   const int& getY() const;
   const int& getStartX() const;
   const int& getStartY() const;
   const double& getPressure() const;
   const double& getRotation() const;
   const QPointF& getVelocity() const;
   const QRectF& getAreaRect() const;
   double getArea() const;

private:
   bool pressed;
   int x;
   int y;
   int startX;
   int startY;
   double pressure;
   double rotation;
   QPointF velocity;
   QRectF area;
};

#ifndef TouchPointHPP
#include "TouchPoint.hpp"
#endif // NOT TouchPointHPP

#endif // NOT TouchPointH
