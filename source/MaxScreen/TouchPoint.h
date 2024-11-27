#ifndef TouchPointH
#define TouchPointH

#include <QMap>

#include <QDataStream>

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
      void save(QDataStream& stream);
      void load(QDataStream& stream);
   };

public:
   TouchPoint();

public:
   bool isPressed() const;
   const double& getX() const;
   const double& getY() const;

private:
   bool pressed;
   double x;
   double y;
};

#ifndef TouchPointHPP
#include "TouchPoint.hpp"
#endif // NOT TouchPointHPP

#endif // NOT TouchPointH
