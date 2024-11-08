#ifndef GraphMaxLineH
#define GraphMaxLineH

#include "GraphAbstractEdge.h"

#include <QJsonObject>

#include "GraphMaxObject.h"

namespace Graph
{
   namespace Max
   {
      class Line : public Abstract::Edge
      {
      public:
         Line(const QJsonObject& lineObject, const Object::IdMap& idMap);

      public:
         int sourceX;
         int sourceY;
         int destX;
         int destY;
         bool isParamLine;
      };
   } // namespace Max
} // namespace Graph

#endif // NOT GraphMaxLineH
