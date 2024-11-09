#ifndef GraphMaxObjectH
#define GraphMaxObjectH

#include "GraphAbstractVertex.h"

#include <QJsonObject>
#include <QRectF>

namespace Graph
{
   namespace Max
   {
      class Object : public Abstract::Vertex
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
} // namespace Graph

#endif // NOT GraphMaxObjectH
