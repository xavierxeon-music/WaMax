#include "GraphMaxLine.h"

#include <QJsonArray>

Graph::Max::Line::Line(const QJsonObject& lineObject, const Object::IdMap& idMap)
   : Abstract::Edge()
   , isParamLine(false)
   , sourceOutlet(0)
   , destInlet(0)
   , sourceX(0)
   , sourceY(0)
   , destX(0)
   , destY(0)
{
   // source
   {
      const QJsonArray sourceArray = lineObject["source"].toArray();
      const QString sourceId = sourceArray.at(0).toString();
      sourceOutlet = sourceArray.at(1).toInt();

      Object* sourceObject = idMap[sourceId];
      sourceObject->outlets.connected.append(sourceOutlet);

      const int outletDist = sourceObject->patchRect.width() / sourceObject->outlets.count;
      sourceX = 10 + sourceObject->patchRect.x() + (outletDist * sourceOutlet);
      sourceY = sourceObject->patchRect.y() + sourceObject->patchRect.height();

      vertexA = sourceObject;
   }

   // destination
   {
      const QJsonArray destArray = lineObject["destination"].toArray();
      const QString destId = destArray.at(0).toString();
      destInlet = destArray.at(1).toInt();

      Object* destObject = idMap[destId];
      destObject->inlets.connected.append(destInlet);

      const int inletDist = destObject->patchRect.width() / destObject->inlets.count;
      destX = 10 + destObject->patchRect.x() + (inletDist * destInlet);
      destY = destObject->patchRect.y();

      vertexB = destObject;
   }
}
