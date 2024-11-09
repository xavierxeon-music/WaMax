#include "GraphMaxPatch.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonValue>

#include "GraphAbstractAlgorithm.h"

Graph::Max::Patch::Patch()
   : Symbolic::Graph<Object, Line>()
   , typeBuffer()
{
}

void Graph::Max::Patch::read(const QString& patchFileName)
{
   typeBuffer.clear();
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

   const Object::IdMap idMap = readObjects(patcherObject);
   readLines(patcherObject, idMap);

   analyse();
}

Graph::Max::Object::List Graph::Max::Patch::findAll(const Object::Type& type) const
{
   return typeBuffer.value(type, Object::List());
}

Graph::Max::Object::List Graph::Max::Patch::findAll(const QList<Object::Type>& typeList) const
{
   Object::List list;
   for (const Object::Type& type : typeList)
   {
      if (!typeBuffer.contains(type))
         continue;

      list.append(typeBuffer.value(type));
   }

   return list;
}

void Graph::Max::Patch::analyse()
{
   static const QList<Object::Type> sourceTypeList = {Object::Type::PatcherArgs, Object::Type::Inlet};
   static const QList<Object::Type> processTypeList = {Object::Type::Route, Object::Type::RoutePass, Object::Type::TypeRoute, Object::Type::Unpack};

   Abstract::Algorithm algo(this);
   const Object::List sources = findAll(sourceTypeList);
   const Object::List processors = findAll(processTypeList);

   for (Object* source : sources)
   {
      const Abstract::Algorithm::Tree tree = algo.breadthFirst(source);
      for (Object* processor : processors)
      {
         const int targetIndex = vertexIndex(processor);
         const Abstract::Algorithm::Path path = tree.compilePath(targetIndex);
         const int depth = path.verticies.count();

         auto pathIsValid = [&]()
         {
            if (0 == depth)
               return false;

            for (int index = 0; index < depth; index++)
            {
               const int vertexIndexA = path.verticies.at(index);
               Object* outletObject = getVertexCast(vertexIndexA);

               if (!sourceTypeList.contains(outletObject->type) && !processTypeList.contains(outletObject->type))
                  return false;
            }
            return true;
         };

         if (!pathIsValid())
            continue;

         for (int index = 1; index < depth; index++)
         {
            const int vertexIndexA = path.verticies.at(index);
            Object* outletObject = getVertexCast(vertexIndexA);

            const int vertexIndexB = path.verticies.at(index - 1);
            Object* inletObject = getVertexCast(vertexIndexB);

            const int edgeIndex = findEdgeIndex(outletObject, inletObject);
            Line* line = getEdgeCast(edgeIndex);
            line->isParamLine = true;
         }
      }
   }
}

Graph::Max::Object::IdMap Graph::Max::Patch::readObjects(const QJsonObject patcherObject)
{
   static const QStringList skipList = {"comment", "panel"};

   Object::IdMap idMap;
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

      typeBuffer[object->type].append(object);

      idMap[object->id] = object;
   }

   return idMap;
}

void Graph::Max::Patch::readLines(const QJsonObject patcherObject, const Object::IdMap& idMap)
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
