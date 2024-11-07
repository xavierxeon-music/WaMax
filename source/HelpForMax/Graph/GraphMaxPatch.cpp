#include "GraphMaxPatch.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonValue>

#include "GraphMaxLine.h"
#include "GraphMaxObject.h"

Graph::Max::Patch::Patch()
   : Symbolic::Graph()
   , idMap()
{
}

void Graph::Max::Patch::read(const QString& patchFileName)
{
   idMap.clear();
   clear(true);

   if (patchFileName.isEmpty())
      return;

   QFile file(patchFileName);
   if (!file.open(QIODevice::ReadOnly))
      return;

   const QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
   file.close();

   QJsonObject object = doc.object();
   if (object.empty())
      return;

   const QJsonObject patcherObject = object["patcher"].toObject();

   readObjects(patcherObject);
   readLines(patcherObject);
}

void Graph::Max::Patch::readObjects(const QJsonObject patcherObject)
{
   static const QStringList skipList = {"comment", "panel"};
   const QJsonArray boxArray = patcherObject["boxes"].toArray();

   for (int index = 0; index < boxArray.size(); index++)
   {
      QJsonObject boxObject = boxArray.at(index).toObject();
      boxObject = boxObject["box"].toObject();

      if (!boxObject.contains("maxclass"))
         continue;

      const QString className = boxObject["maxclass"].toString();
      if (skipList.contains(className))
         continue;

      Object* object = new Object(boxObject);
      addVertex(object);

      const QString id = boxObject["id"].toString();
      idMap[id] = object;
   }
}

void Graph::Max::Patch::readLines(const QJsonObject patcherObject)
{
   const QJsonArray lineArray = patcherObject["lines"].toArray();

   for (int index = 0; index < lineArray.size(); index++)
   {
      QJsonObject lineObject = lineArray.at(index).toObject();
      lineObject = lineObject["patchline"].toObject();

      Line* line = new Line(lineObject, idMap);
      addEdge(line);
   }
}
