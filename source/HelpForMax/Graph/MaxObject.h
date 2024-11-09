#ifndef MaxObjectH
#define MaxObjectH

#include "StructureVertex.h"

#include <QJsonObject>
#include <QRectF>

namespace Max
{
   class Object : public Structure::Vertex
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
   };
} // namespace Max

#endif // NOT MaxObjectH
