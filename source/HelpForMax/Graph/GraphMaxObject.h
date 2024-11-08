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
            TypeRoute
         };

      public:
         Object(const QJsonObject& boxObject);

      public:
         QRectF patchRect;
         QString className;
         QString text;
         QString comment;
         int inletCount;
         int outletCount;
         Type type;
      };
   } // namespace Max
} // namespace Graph

#endif // NOT GraphMaxObjectH
