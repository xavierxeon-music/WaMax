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

      public:
         Object(const QJsonObject& boxObject);

      public:
         QRectF patchRect;
         QString className;
         QString text;
         QString comment;
         int inletCount;
         int outletCount;
      };
   } // namespace Max
} // namespace Graph

#endif // NOT GraphMaxObjectH
