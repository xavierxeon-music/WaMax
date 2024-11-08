#include "GraphMaxPatch.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonValue>

#include "GraphAbstractAlgorithm.h"

Graph::Max::Patch::Patch()
   : Symbolic::Graph<Object, Line>()
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

   analyse();
}

Graph::Max::Object::List Graph::Max::Patch::findAll(const Object::Type& type) const
{
   const QList<Object::Type> typeList = {type};
   return findAll(typeList);
}

Graph::Max::Object::List Graph::Max::Patch::findAll(const QList<Object::Type>& typeList) const
{
   Object::List list;
   for (int vertIndex = 0; vertIndex < vertexCount(); vertIndex++)
   {
      Max::Object* object = getVertexCast(vertIndex);
      if (typeList.contains(object->type))
         list.append(object);
   }
   return list;
}

void Graph::Max::Patch::analyse()
{
   static const QList<Object::Type> sourceTypeList = {Object::Type::PatcherArgs, Object::Type::Inlet};
   static const QList<Object::Type> targetTypeList = {Object::Type::Route, Object::Type::RoutePass, Object::Type::TypeRoute};

   Abstract::Algorithm algo(this);
   const Object::List sources = findAll(sourceTypeList);
   const Object::List targets = findAll(targetTypeList);

   for (Object* source : sources)
   {
      const Abstract::Algorithm::Tree tree = algo.depthFirst(source);
      for (Object* target : targets)
      {
         const int targetIndex = vertexIndex(target);
         const Abstract::Algorithm::Path path = tree.compilePath(targetIndex);
         const int depth = path.verticies.count();
         if (0 == depth)
            continue;
         for (int index = 1; index < path.verticies.count(); index++)
         {
            const int vertexIndexA = path.verticies.at(index);
            const int vertexIndexB = path.verticies.at(index - 1);

            Object* objectA = getVertexCast(vertexIndexA);
            Object* objectB = getVertexCast(vertexIndexB);

            const int edgeIndex = findEdgeIndex(objectA, objectB);
            Line* line = getEdgeCast(edgeIndex);
            line->isParamLine = true;
         }
      }
   }
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
