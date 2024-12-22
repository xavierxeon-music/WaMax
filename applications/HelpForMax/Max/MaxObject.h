#ifndef MaxObjectH
#define MaxObjectH

#include "DiscreteMathsVertex.h"

#include <QJsonObject>
#include <QRectF>

namespace Max
{
   class Object : public DiscreteMaths::Vertex
   {
   public:
      using List = QList<Object*>;
      using IdMap = QMap<QString, Object*>;

      enum class Type
      {
         Other,
         Inlet,
         Outlet,
         PatcherArgs,
         Route,
         RoutePass,
         TypeRoute,
         Unpack
      };

      struct Ports
      {
         int count = 0;
         QList<int> connected;
      };

   public:
      Object(const QJsonObject& boxObject);

   public:
      QRectF patchRect;
      QString className;
      QString text;
      QString comment;
      Ports inlets;
      Ports outlets;
      Type type;
      QString id;
      bool isParamObject;
   };
} // namespace Max

#endif // NOT MaxObjectH
