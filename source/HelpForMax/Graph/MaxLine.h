#ifndef MaxLineH
#define MaxLineH

#include "StructureEdge.h"

#include <QJsonObject>

#include "MaxObject.h"

namespace Max
{
   class Line : public Structure::Edge
   {
   public:
      Line(const QJsonObject& lineObject, const Object::IdMap& idMap);

   public:
      bool isParamLine;
      int sourceOutlet;
      int destInlet;

      int sourceX;
      int sourceY;
      int destX;
      int destY;
   };
} // namespace Max

#endif // NOT MaxLineH
