#ifndef GraphMaxObjectH
#define GraphMaxObjectH

#include "GraphSymbolicVertex.h"

#include <QJsonObject>
#include <QRectF>

namespace Graph
{
   namespace Max
   {
      class Object : public Symbolic::Vertex
      {
      public:
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

         using TypeMap = QMap<QString, Type>;

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
