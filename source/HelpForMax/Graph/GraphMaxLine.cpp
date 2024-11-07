#include "GraphMaxLine.h"

#include <QJsonArray>

Graph::Max::Line::Line(const QJsonObject& lineObject, const Object::IdMap& idMap)
   : Symbolic::Edge()
   , sourceX(0)
   , sourceY(0)
   , destX(0)
   , destY(0)
{
   const QJsonArray sourceArray = lineObject["source"].toArray();
   const QString sourceId = sourceArray.at(0).toString();
   /*
   if (!idMap.contains(sourceId))
      continue;
   */

   Object* sourceObject = idMap[sourceId];

   const int sourceOffset = sourceArray.at(1).toInt();
   const QRectF sourceRect = QRectF(sourceObject->patchRect.x(), sourceObject->patchRect.y(), sourceObject->patchRect.width(), sourceObject->patchRect.height());

   const int outletDist = sourceRect.width() / sourceObject->outletCount;
   sourceX = 10 + sourceRect.x() + (outletDist * sourceOffset);
   sourceY = sourceRect.y() + sourceRect.height();

   const QJsonArray destArray = lineObject["destination"].toArray();
   const QString destId = destArray.at(0).toString();
   /*
   if (!idMap.contains(destId))
      continue;
   */
   Object* destObject = idMap[destId];
   const int destOffset = destArray.at(1).toInt();
   const QRectF destRect = QRectF(destObject->patchRect.x(), destObject->patchRect.y(), destObject->patchRect.width(), destObject->patchRect.height());

   const int inletDist = destRect.width() / destObject->inletCount;
   destX = 10 + destRect.x() + (inletDist * destOffset);
   destY = destRect.y();
}
