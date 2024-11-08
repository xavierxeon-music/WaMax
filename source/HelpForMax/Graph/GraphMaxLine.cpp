#include "GraphMaxLine.h"

#include <QJsonArray>

Graph::Max::Line::Line(const QJsonObject& lineObject, const Object::IdMap& idMap)
   : Abstract::Edge()
   , sourceX(0)
   , sourceY(0)
   , destX(0)
   , destY(0)
   , isParamLine(false)
{
   // source
   {
      const QJsonArray sourceArray = lineObject["source"].toArray();
      const QString sourceId = sourceArray.at(0).toString();

      Object* sourceObject = idMap[sourceId];
      const int sourceOffset = sourceArray.at(1).toInt();

      const int outletDist = sourceObject->patchRect.width() / sourceObject->outletCount;
      sourceX = 10 + sourceObject->patchRect.x() + (outletDist * sourceOffset);
      sourceY = sourceObject->patchRect.y() + sourceObject->patchRect.height();

      vertexA = sourceObject;
   }

   // destination
   {
      const QJsonArray destArray = lineObject["destination"].toArray();
      const QString destId = destArray.at(0).toString();

      Object* destObject = idMap[destId];
      const int destOffset = destArray.at(1).toInt();

      const int inletDist = destObject->patchRect.width() / destObject->inletCount;
      destX = 10 + destObject->patchRect.x() + (inletDist * destOffset);
      destY = destObject->patchRect.y();

      vertexB = destObject;
   }
}
