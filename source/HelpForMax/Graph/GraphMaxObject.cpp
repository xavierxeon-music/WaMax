#include "GraphMaxObject.h"

#include <QJsonArray>

Graph::Max::Object::Object(const QJsonObject& boxObject)
   : Symbolic::Vertex()
   , patchRect()
   , className()
   , text()
   , comment()
   , inletCount(0)
   , outletCount(0)
{
   className = boxObject["maxclass"].toString();

   QJsonArray patchRectData = boxObject["patching_rect"].toArray();
   patchRect.setX(patchRectData[0].toDouble());
   patchRect.setY(patchRectData[1].toDouble());
   patchRect.setWidth(patchRectData[2].toDouble());
   patchRect.setHeight(patchRectData[3].toDouble());

   text = boxObject["text"].toString().simplified();
   if ("inlet" == className)
   {
      const int index = boxObject["index"].toInt();
      text = "IN\n" + QString::number(index);

      comment = boxObject["comment"].toString();
   }
   else if ("outlet" == className)
   {
      const int index = boxObject["index"].toInt();
      text = "OUT\n" + QString::number(index);

      comment = boxObject["comment"].toString();
   }
   if (text.isEmpty())
      text = className;

   inletCount = boxObject["numinlets"].toInt();
   outletCount = boxObject["numoutlets"].toInt();
}
