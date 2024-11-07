#ifndef GraphMaxLineH
#define GraphMaxLineH

#include "GraphSymbolicEdge.h"

#include <QJsonObject>

#include "GraphMaxObject.h"

namespace Graph
{
   namespace Max
   {
      class Line : public Symbolic::Edge
      {
      public:
         Line(const QJsonObject& lineObject, const Object::IdMap& idMap);

      public:
         int sourceX;
         int sourceY;
         int destX;
         int destY;
      };
   } // namespace Max
} // namespace Graph

#endif // NOT GraphMaxLineH
