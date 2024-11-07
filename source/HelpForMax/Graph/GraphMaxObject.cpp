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
   , type(Type::Other)
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
      type = Type::Inlet;
   }
   else if ("outlet" == className)
   {
      const int index = boxObject["index"].toInt();
      text = "OUT\n" + QString::number(index);
      comment = boxObject["comment"].toString();
      type = Type::Outlet;
   }
   else if (text.isEmpty())
   {
      text = className;
      qDebug() << "NO TEXT" << className;
   }
   else if ("newobj" == className)
   {
      static const TypeMap typeMap = {{"patcherargs", Type::PatcherArgs},
                                      {"route", Type::Route},
                                      {"routepass", Type::RoutePass},
                                      {"typeroute~", Type::TypeRoute}};

      const QString typeString = text.split(" ", Qt::SkipEmptyParts).first();
      type = typeMap.value(typeString, Type::Other);
   }

   inletCount = boxObject["numinlets"].toInt();
   outletCount = boxObject["numoutlets"].toInt();
}
